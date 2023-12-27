#include <dnd_config.hpp>

#include "character_data.hpp"

#include <array>
#include <memory>
#include <unordered_set>
#include <vector>

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
#include <core/validation/character/progression_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

CharacterData::CharacterData() noexcept
    : ValidationData(), features_data(), base_ability_scores_data(this), feature_providers_data(this),
      progression_data(this), decisions_data() {}

std::unique_ptr<ValidationData> CharacterData::pack() const { return std::make_unique<CharacterData>(*this); }

Errors CharacterData::validate() const {
    Errors errors = ValidationData::validate();
    std::unordered_set<std::string> unique_feature_names;
    for (const FeatureData& feature_data : features_data) {
        errors += feature_data.validate();
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Character has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    errors += base_ability_scores_data.validate();
    errors += feature_providers_data.validate();
    errors += progression_data.validate();
    for (const DecisionData& decision_data : decisions_data) {
        errors += decision_data.validate();
    }
    return errors;
}

Errors CharacterData::validate_relations(const Content& content) const {
    Errors errors;
    if (content.get_characters().contains(name)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, this,
            fmt::format("Character has duplicate name \"{}\".", name)
        );
    }
    for (const FeatureData& feature_data : features_data) {
        errors += feature_data.validate_relations(content);
        if (content.get_features().contains(feature_data.name)
            || content.get_class_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }

    errors += base_ability_scores_data.validate_relations(content);
    errors += feature_providers_data.validate_relations(content);

    OptCRef<Class> class_optional = content.get_classes().get(feature_providers_data.class_name);
    if (!feature_providers_data.class_name.empty() && class_optional.has_value()) {
        const Class& cls = class_optional.value();
        if (progression_data.level >= cls.get_important_levels().get_subclass_level()
            && feature_providers_data.subclass_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INCONSISTENT_ATTRIBUTES, this,
                fmt::format(
                    "{}s of level {} must have a subclass and {} is level {}.", cls.get_name(),
                    cls.get_important_levels().get_subclass_level(), name, progression_data.level
                )
            );
        } else if (progression_data.level < cls.get_important_levels().get_subclass_level() && !feature_providers_data.subclass_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INCONSISTENT_ATTRIBUTES, this,
                fmt::format(
                    "{}s cannot have a subclass before level {} and {} is only level {}.", cls.get_name(),
                    cls.get_important_levels().get_subclass_level(), name, progression_data.level
                )
            );
        }

        for (int hit_dice_roll : progression_data.hit_dice_rolls) {
            if (!cls.get_hit_dice().value_is_possible(hit_dice_roll)) {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE, this,
                    fmt::format(
                        "Character has invalid hit dice roll of {} (must be between {} and {}).", hit_dice_roll,
                        cls.get_hit_dice().min_value(), cls.get_hit_dice().max_value()
                    )
                );
            }
        }
    }

    for (const DecisionData& decision_data : decisions_data) {
        errors += decision_data.validate_relations(content);
    }

    // TODO: recheck this function, the code that was here did nothing

    return errors;
}

} // namespace dnd
