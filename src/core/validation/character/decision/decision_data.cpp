#include <dnd_config.hpp>

#include "decision_data.hpp"

#include <algorithm>
#include <cassert>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

static Errors validate_decision_raw(const Decision::Data& data) {
    Errors errors;
    if (data.get_target() == nullptr) {
        errors.add_validation_error(
            ValidationError::Code::MISSING_ATTRIBUTE, "Decision has no target that it belongs to."
        );
    }
    if (data.feature_name.empty()) {
        errors.add_validation_error(ValidationError::Code::MISSING_ATTRIBUTE, "Decision's feature name is empty.");
    }
    if (data.selections.empty()) {
        errors.add_validation_error(ValidationError::Code::MISSING_ATTRIBUTE, "Decision's selections are empty.");
    }

    return errors;
}

static Errors validate_decision_relations(const Decision::Data& data, const Content& content) {
    Errors errors;
    if (data.get_target() == nullptr) {
        return errors;
    }
    const Effects* target = data.get_target();
    std::map<std::string, const Choice*> choices;
    for (const Choice& choice : target->get_choices()) {
        assert(!choices.contains(choice.get_attribute_name()));
        if (data.selections.contains(choice.get_attribute_name())) {
            choices[choice.get_attribute_name()] = &choice;
        } else {
            errors.add_validation_error(
                ValidationError::Code::MISSING_ATTRIBUTE,
                fmt::format("Decision's selection is missing a decision for '{}'.", choice.get_attribute_name())
            );
        }
    }
    for (const auto& [attribute_name, selection] : data.selections) {
        if (!choices.contains(attribute_name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format(
                    "Decision's selection '{}' is not allowed by the choice it is referring to.", attribute_name
                )
            );
            continue;
        }
        const std::set<std::string> possible_values = choices[attribute_name]->possible_values(content);
        for (const std::string& value : selection) {
            if (std::find(possible_values.cbegin(), possible_values.cend(), value) == possible_values.cend()) {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                    fmt::format("Decision's selection '{}' has an invalid value '{}'.", attribute_name, value)
                );
            }
        }
    }
    return errors;
}

static Errors validate_decision_target_for_character_and_content(
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
                const std::string& species_name = character_data.feature_providers_data.species_name;
                OptCRef<Species> species_optional = content.get_species().get(species_name);
                if (species_optional.has_value()) {
                    const Species& species = species_optional.value();
                    feature_found = std::any_of(
                        species.get_features().begin(), species.get_features().end(), has_feature_name
                    );
                    if (feature_found) {
                        break;
                    }
                }
                const std::string& subspecies_name = character_data.feature_providers_data.subspecies_name;
                OptCRef<Subspecies> subspecies_optional = content.get_subspecies().get(subspecies_name);
                if (subspecies_optional.has_value()) {
                    const Subspecies& subspecies = subspecies_optional.value();
                    feature_found = std::any_of(
                        subspecies.get_features().begin(), subspecies.get_features().end(), has_feature_name
                    );
                }
                break;
            }
            case 1: /* ClassFeature */ {
                const std::string& class_name = character_data.feature_providers_data.class_name;
                OptCRef<Class> class_optional = content.get_classes().get(class_name);
                if (class_optional.has_value()) {
                    const Class& cls = class_optional.value();
                    feature_found = std::any_of(cls.get_features().begin(), cls.get_features().end(), has_feature_name);
                    if (feature_found) {
                        break;
                    }
                }
                const std::string& subclass_name = character_data.feature_providers_data.subclass_name;
                OptCRef<Subclass> subclass_optional = content.get_subclasses().get(subclass_name);
                if (content.get_subclasses().contains(subclass_name)) {
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

Errors validate_decision_for_character_and_content(
    const Decision::Data& data, const Character::Data& character_data, const Content& content
) {
    Errors errors = validate_decision_raw(data);
    errors += validate_decision_relations(data, content);
    errors += validate_decision_target_for_character_and_content(data, character_data, content);
    return errors;
}

} // namespace dnd
