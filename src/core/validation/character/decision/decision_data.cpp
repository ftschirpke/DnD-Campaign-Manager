#include <algorithm>
#include <dnd_config.hpp>

#include "decision_data.hpp"

#include <cassert>
#include <map>
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
    for (const auto& choice : target->get_choices()) {
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
        for (const auto& value : selection) {
            if (std::find(possible_values.cbegin(), possible_values.cend(), value) == possible_values.cend()) {
                errors.add_validation_error(
                    ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                    fmt::format("Decision's selection '{}' has an invalid value '{}'.", attribute_name, value)
                );
            }
        }
    }

    std::optional<EffectsProviderType> feature_type_optional = content.contains_effects_provider(feature_name);
    if (!feature_type_optional.has_value()) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, "Decision cannot be linked to any existing feature."
        );
        return errors;
    }

    bool target_exists = false;
    switch (feature_type_optional.value()) {
        case EffectsProviderType::Feature:
        case EffectsProviderType::Choosable: {
            const EffectsProvider& linked_effects_provider = content.get_effects_provider(feature_name).value();
            const Effects& effects = linked_effects_provider.get_main_effects();
            target_exists = &effects == target;
            break;
        }
        case EffectsProviderType::ClassFeature: {
            const ClassFeature& linked_class_feature = content.get_class_features().get(feature_name).value();
            const Effects& main_effects = linked_class_feature.get_main_effects();
            if (&main_effects != target) {
                bool found_in_higher_levels = false;
                for (const auto& [_, effects] : linked_class_feature.get_higher_level_effects()) {
                    if (&effects == target) {
                        found_in_higher_levels = true;
                        break;
                    }
                }
                if (found_in_higher_levels) {
                    target_exists = true;
                }
            } else {
                target_exists = true;
            }
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
        switch (feature_type_optional.value()) {
            // TODO: check character specific features for choices
            case EffectsProviderType::Feature: {
                const std::string& race_name = character_data->character_basis_data.race_name;
                auto race_optional = content.get_character_races().get(race_name);
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
                auto subrace_optional = content.get_character_subraces().get(subrace_name);
                if (subrace_optional.has_value()) {
                    const CharacterSubrace& subrace = subrace_optional.value();
                    feature_found = std::any_of(
                        subrace.get_features().begin(), subrace.get_features().end(), has_feature_name
                    );
                }
                break;
            }
            case EffectsProviderType::ClassFeature: {
                const std::string& class_name = character_data->character_basis_data.class_name;
                auto class_optional = content.get_character_classes().get(class_name);
                if (class_optional.has_value()) {
                    const CharacterClass& cls = class_optional.value();
                    feature_found = std::any_of(cls.get_features().begin(), cls.get_features().end(), has_feature_name);
                    if (feature_found) {
                        break;
                    }
                }
                const std::string& subclass_name = character_data->character_basis_data.subclass_name;
                auto subclass_optional = content.get_character_subclasses().get(subclass_name);
                if (content.get_character_subclasses().contains(subclass_name)) {
                    const CharacterSubclass& subclass = subclass_optional.value();
                    feature_found = std::any_of(
                        subclass.get_features().begin(), subclass.get_features().end(), has_feature_name
                    );
                }
                break;
            }
            case EffectsProviderType::Choosable: {
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
