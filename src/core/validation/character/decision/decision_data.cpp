#include <dnd_config.hpp>

#include "core/models/character_subrace/character_subrace.hpp"
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
#include <core/validation/character/character_data.hpp>
#include <core/validation/effects/effects_data.hpp>

dnd::DecisionData::DecisionData(const dnd::CharacterData* parent, const dnd::Effects* target) noexcept
    : ValidationSubdata(parent), selections(), character_data(parent), target(target) {}

dnd::Errors dnd::DecisionData::validate() const {
    Errors errors;
    if (parent == nullptr) {
        errors.add_validation_error(
            ValidationErrorCode::MISSING_ATTRIBUTE, parent, "Decision has no character that it belongs to."
        );
    }
    if (target == nullptr) {
        errors.add_validation_error(
            ValidationErrorCode::MISSING_ATTRIBUTE, parent, "Decision has no target that it belongs to."
        );
    }
    if (feature_name.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::MISSING_ATTRIBUTE, parent, "Decision's feature name is empty."
        );
    }
    if (selections.empty()) {
        errors.add_validation_error(ValidationErrorCode::MISSING_ATTRIBUTE, parent, "Decision's selections are empty.");
    }

    return errors;
}

dnd::Errors dnd::DecisionData::validate_relations(const dnd::Content& content) const {
    Errors errors;

    if (target == nullptr) {
        return errors;
    }
    std::map<std::string, const dnd::Choice*> choices;
    for (const Choice& choice : target->get_choices()) {
        assert(!choices.contains(choice.get_attribute_name()));
        if (selections.contains(choice.get_attribute_name())) {
            choices[choice.get_attribute_name()] = &choice;
        } else {
            errors.add_validation_error(
                ValidationErrorCode::MISSING_ATTRIBUTE, parent,
                fmt::format("Decision's selection is missing a decision for '{}'.", choice.get_attribute_name())
            );
        }
    }
    for (const auto& [attribute_name, selection] : selections) {
        if (!choices.contains(attribute_name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
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
                    ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                    fmt::format("Decision's selection '{}' has an invalid value '{}'.", attribute_name, value)
                );
            }
        }
    }


    std::optional<EffectsProviderVariant> effects_provider_optional = content.get_effects_provider(feature_name);
    if (!effects_provider_optional.has_value()) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, "Decision cannot be linked to any existing feature."
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
                    if (&effects == target) {
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
    if (character_data == nullptr) {
        errors.add_validation_error(
            ValidationErrorCode::MISSING_ATTRIBUTE, parent, "Decision has no character data to validate against."
        );
    } else if (target_exists) {
        auto has_feature_name = [this](const ContentPiece& effects_provider) {
            return effects_provider.get_name() == feature_name;
        };
        switch (effects_provider_variant.index()) {
            // TODO: check character specific features for choices
            case 0: /* Feature */ {
                const std::string& race_name = character_data->character_basis_data.race_name;
                OptCRef<CharacterRace> race_optional = content.get_character_races().get(race_name);
                if (race_optional.has_value()) {
                    const CharacterRace& race = race_optional.value();
                    feature_found = std::any_of(
                        race.get_features().begin(), race.get_features().end(), has_feature_name
                    );
                    if (feature_found) {
                        break;
                    }
                }
                const std::string& subrace_name = character_data->character_basis_data.subrace_name;
                OptCRef<CharacterSubrace> subrace_optional = content.get_character_subraces().get(subrace_name);
                if (subrace_optional.has_value()) {
                    const CharacterSubrace& subrace = subrace_optional.value();
                    feature_found = std::any_of(
                        subrace.get_features().begin(), subrace.get_features().end(), has_feature_name
                    );
                }
                break;
            }
            case 1: /* ClassFeature */ {
                const std::string& class_name = character_data->character_basis_data.class_name;
                OptCRef<CharacterClass> class_optional = content.get_character_classes().get(class_name);
                if (class_optional.has_value()) {
                    const CharacterClass& cls = class_optional.value();
                    feature_found = std::any_of(cls.get_features().begin(), cls.get_features().end(), has_feature_name);
                    if (feature_found) {
                        break;
                    }
                }
                const std::string& subclass_name = character_data->character_basis_data.subclass_name;
                OptCRef<CharacterSubclass> subclass_optional = content.get_character_subclasses().get(subclass_name);
                if (content.get_character_subclasses().contains(subclass_name)) {
                    const CharacterSubclass& subclass = subclass_optional.value();
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
                ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format(
                    "Decision target \"{}\" exists but is not available to the character ({}).", feature_name,
                    character_data->name
                )
            );
        }
    } else {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent,
            fmt::format("Decision has target feature \"{}\" which doesn't seem to exist.", feature_name)
        );
    }
    return errors;
}

const dnd::CharacterData* dnd::DecisionData::get_character_data() const noexcept { return character_data; }

const dnd::Effects* dnd::DecisionData::get_target() const noexcept { return target; }

void dnd::DecisionData::set_target(const dnd::Effects* new_target) noexcept { target = new_target; }
