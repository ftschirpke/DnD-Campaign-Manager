#include <dnd_config.hpp>

#include "character_validation.hpp"
#include "core/validation/character/decision/decision_validation.hpp"

#include <unordered_set>

#include <fmt/format.h>

#include <core/basic_mechanics/dice.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/species/species.hpp>
#include <core/referencing_content_library.hpp>
#include <core/validation/character/ability_scores_validation.hpp>
#include <core/validation/character/feature_providers_validation.hpp>
#include <core/validation/character/progression_validation.hpp>
#include <core/validation/effects_provider/feature_validation.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

static Errors validate_character_raw_nonrecursively(const Character::Data& data) {
    Errors errors = validate_name_description_and_source(data);
    std::unordered_set<std::string> unique_feature_names;
    for (const Feature::Data& feature_data : data.features_data) {
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Character has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    return errors;
}

static Errors validate_character_relations_nonrecursively(const Character::Data& data, const Content& content) {
    Errors errors;
    if (content.get_characters().contains(data.name)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Character has duplicate name \"{}\".", data.name)
        );
    }
    for (const Feature::Data& feature_data : data.features_data) {
        if (content.get_features().contains(feature_data.name)
            || content.get_class_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }

    OptCRef<Class> class_optional = content.get_classes().get(data.feature_providers_data.class_name);
    if (!data.feature_providers_data.class_name.empty() && class_optional.has_value()) {
        const Class& cls = class_optional.value();
        if (data.progression_data.level >= cls.get_important_levels().get_subclass_level()
            && data.feature_providers_data.subclass_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INCONSISTENT_ATTRIBUTES,
                fmt::format(
                    "{}s of level {} must have a subclass and {} is level {}.", cls.get_name(),
                    cls.get_important_levels().get_subclass_level(), data.name, data.progression_data.level
                )
            );
        } else if (data.progression_data.level < cls.get_important_levels().get_subclass_level() && !data.feature_providers_data.subclass_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INCONSISTENT_ATTRIBUTES,
                fmt::format(
                    "{}s cannot have a subclass before level {} and {} is only level {}.", cls.get_name(),
                    cls.get_important_levels().get_subclass_level(), data.name, data.progression_data.level
                )
            );
        }

        for (int hit_dice_roll : data.progression_data.hit_dice_rolls) {
            if (!cls.get_hit_dice().value_is_possible(hit_dice_roll)) {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                    fmt::format(
                        "Character has invalid hit dice roll of {} (must be between {} and {}).", hit_dice_roll,
                        cls.get_hit_dice().min_value(), cls.get_hit_dice().max_value()
                    )
                );
            }
        }
    }
    // TODO: recheck this function, the code that was here did nothing
    return errors;
}

Errors validate_character_nonrecursively_for_content(const Character::Data& data, const Content& content) {
    Errors errors = validate_character_raw_nonrecursively(data);
    errors += validate_character_relations_nonrecursively(data, content);
    return errors;
}

Errors validate_character_recursively_for_content(const Character::Data& data, const Content& content) {
    Errors errors = validate_character_nonrecursively_for_content(data, content);
    for (const Feature::Data& feature_data : data.features_data) {
        errors += validate_feature_recursively_for_content(feature_data, content);
    }
    errors += validate_ability_scores(data.base_ability_scores_data);
    errors += validate_feature_providers_for_content(data.feature_providers_data, content);
    errors += validate_progression(data.progression_data);
    for (const Decision::Data& decision_data : data.decisions_data) {
        errors += validate_decision_for_character_and_content(decision_data, data, content);
    }
    return errors;
}

} // namespace dnd
