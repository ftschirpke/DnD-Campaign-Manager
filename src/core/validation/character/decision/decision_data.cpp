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
#include <core/models/effect_holder/choice/choice.hpp>
#include <core/models/effect_holder/effect_holder.hpp>
#include <core/models/feature_holder/feature_holder.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>

dnd::DecisionData::DecisionData(const dnd::CharacterData* parent, const dnd::EffectHolder* target) noexcept
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

bool feature_is_in(const dnd::Feature* feature_to_find, const dnd::FeatureHolder& feature_holder) {
    for (const auto& feature : feature_holder.get_features()) {
        if (&feature == feature_to_find) {
            return true;
        }
    }
    return false;
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

    if (!content.get_features().contains(feature_name)) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, "Decision cannot be linked to any existing feature."
        );
        return errors;
    }

    const Feature* linked_feature = content.get_features().get(feature_name);

    if (&linked_feature->get_main_part() != target) {
        bool target_found_in_feature = false;
        for (const auto& part : linked_feature->get_other_parts()) {
            if (&part == target) {
                target_found_in_feature = true;
                break;
            }
        }
        if (!target_found_in_feature) {
            errors.add_validation_error(
                ValidationErrorCode::RELATION_NOT_FOUND, parent,
                "Decision cannot be linked to any of the feature's parts."
            );
        }
    }

    bool feature_found = false;
    if (character_data != nullptr) {
        if (content.get_character_races().contains(character_data->character_basis_data.race_name)) {
            feature_found = feature_is_in(
                linked_feature, content.get_character_races().get(character_data->character_basis_data.race_name)
            );
        }
        if (!feature_found
            && content.get_character_subraces().contains(character_data->character_basis_data.subrace_name)) {
            feature_found = feature_is_in(
                linked_feature, content.get_character_subraces().get(character_data->character_basis_data.subrace_name)
            );
        }
        if (!feature_found
            && content.get_character_classes().contains(character_data->character_basis_data.class_name)) {
            feature_found = feature_is_in(
                linked_feature, content.get_character_classes().get(character_data->character_basis_data.class_name)
            );
        }
        if (!feature_found
            && content.get_character_subclasses().contains(character_data->character_basis_data.subclass_name)) {
            feature_found = feature_is_in(
                linked_feature,
                content.get_character_subclasses().get(character_data->character_basis_data.subclass_name)
            );
        }
    }
    if (!feature_found) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent,
            "Decision cannot be linked to any of the character's features."
        );
    }
    return errors;
}

const dnd::CharacterData* dnd::DecisionData::get_character_data() const noexcept { return character_data; }

const dnd::EffectHolder* dnd::DecisionData::get_target() const noexcept { return target; }

void dnd::DecisionData::set_target(const dnd::EffectHolder* new_target) noexcept { target = new_target; }
