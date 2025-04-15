#include <dnd_config.hpp>

#include "character_validation.hpp"

#include <unordered_set>

#include <fmt/format.h>

#include <core/basic_mechanics/dice.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/species/species.hpp>
#include <core/referencing_content_library.hpp>
#include <core/validation/character/ability_scores_validation.hpp>
#include <core/validation/character/decision_validation.hpp>
#include <core/validation/character/feature_providers_validation.hpp>
#include <core/validation/character/progression_validation.hpp>
#include <core/validation/effects_provider/feature_validation.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

static Errors validate_decision_target_for_character(
    const Decision::Data& data, const Character::Data& character_data, const Content& content
) {
    Errors errors;
    const Effects* target = data.get_target();
    std::optional<EffectsProviderVariant> effects_provider_optional = content.get_effects_provider(data.feature_name);
    if (!effects_provider_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, "Decision cannot be linked to any existing feature."
        );
        return errors;
    }
    bool target_exists = false;
    EffectsProviderVariant effects_provider_variant = effects_provider_optional.value();
    switch (effects_provider_variant.index()) {
        case 0: {
            const Feature& feature = std::get<0>(effects_provider_variant);
            target_exists = target == &feature.get_main_effects();
            break;
        }
        case 1: {
            const ClassFeature& class_feature = std::get<1>(effects_provider_variant);
            if (target == &class_feature.get_main_effects()) {
                target_exists = true;
            } else {
                for (const auto& [_, effects] : class_feature.get_higher_level_effects()) {
                    if (target == &effects) {
                        target_exists = true;
                        break;
                    }
                }
            }
            break;
        }
        case 2: {
            const Choosable& choosable = std::get<2>(effects_provider_variant);
            target_exists = target == &choosable.get_main_effects();
            break;
        }
    }

    bool feature_found = false;
    if (target_exists) {
        auto has_feature_name = [&data](const ContentPiece& effects_provider) {
            return effects_provider.get_name() == data.feature_name;
        };
        switch (effects_provider_variant.index()) {
            // TODO: check character specific features for choices
            case 0: /* Feature */ {
                const std::string& species_key = character_data.feature_providers_data.species_key;
                OptCRef<Species> species_optional = content.get_species().get(species_key);
                if (species_optional.has_value()) {
                    const Species& species = species_optional.value();
                    feature_found = std::any_of(
                        species.get_features().begin(), species.get_features().end(), has_feature_name
                    );
                    if (feature_found) {
                        break;
                    }
                }
                const std::string& subspecies_key = character_data.feature_providers_data.subspecies_key;
                OptCRef<Subspecies> subspecies_optional = content.get_subspecies().get(subspecies_key);
                if (subspecies_optional.has_value()) {
                    const Subspecies& subspecies = subspecies_optional.value();
                    feature_found = std::any_of(
                        subspecies.get_features().begin(), subspecies.get_features().end(), has_feature_name
                    );
                }
                break;
            }
            case 1: /* ClassFeature */ {
                const std::string& class_key = character_data.feature_providers_data.class_key;
                OptCRef<Class> class_optional = content.get_classes().get(class_key);
                if (class_optional.has_value()) {
                    const Class& cls = class_optional.value();
                    feature_found = std::any_of(cls.get_features().begin(), cls.get_features().end(), has_feature_name);
                    if (feature_found) {
                        break;
                    }
                }
                const std::string& subclass_key = character_data.feature_providers_data.subclass_key;
                OptCRef<Subclass> subclass_optional = content.get_subclasses().get(subclass_key);
                if (content.get_subclasses().contains(subclass_key)) {
                    const Subclass& subclass = subclass_optional.value();
                    feature_found = std::any_of(
                        subclass.get_features().begin(), subclass.get_features().end(), has_feature_name
                    );
                }
                break;
            }
            case 2: /* Choosable */ {
                // TODO: check choosables for choices
                break;
            }
        }
        if (!feature_found) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format(
                    "Decision target \"{}\" exists but is not available to the character ({}).", data.feature_name,
                    character_data.name
                )
            );
        }
    } else {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND,
            fmt::format("Decision has target feature \"{}\" which doesn't seem to exist.", data.feature_name)
        );
    }
    return errors;
}

static Errors validate_character_raw_nonrecursively(const Character::Data& data) {
    Errors errors = validate_name_description_and_source(data);
    return errors;
}

static Errors validate_character_relations_nonrecursively(const Character::Data& data, const Content& content) {
    Errors errors;
    if (content.get_characters().contains(data.get_key())) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Character has duplicate key \"{}\".", data.get_key())
        );
    }
    for (const Feature::Data& feature_data : data.features_data) {
        if (content.get_features().contains(feature_data.get_key())
            || content.get_class_features().contains(feature_data.get_key())) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Feature has duplicate key \"{}\".", feature_data.get_key())
            );
        }
    }

    OptCRef<Class> class_optional = content.get_classes().get(data.feature_providers_data.class_key);
    if (!data.feature_providers_data.class_key.empty() && class_optional.has_value()) {
        const Class& cls = class_optional.value();
        if (data.progression_data.level >= cls.get_important_levels().get_subclass_level()
            && data.feature_providers_data.subclass_key.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INCONSISTENT_ATTRIBUTES,
                fmt::format(
                    "{}s of level {} must have a subclass and {} is level {}.", cls.get_name(),
                    cls.get_important_levels().get_subclass_level(), data.name, data.progression_data.level
                )
            );
        } else if (data.progression_data.level < cls.get_important_levels().get_subclass_level()
                   && !data.feature_providers_data.subclass_key.empty()) {
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

    std::map<const Choice* const, std::vector<Decision::Data*>> decisions_by_choices;
    for (const Decision::Data& decision_data : data.decisions_data) {
        Errors decision_errors = validate_decision_for_content(decision_data, content);
        if (decision_errors.ok()) {
            decision_errors += validate_decision_target_for_character(decision_data, data, content);
        }
        errors += std::move(decision_errors);
    }
    return errors;
}

} // namespace dnd
