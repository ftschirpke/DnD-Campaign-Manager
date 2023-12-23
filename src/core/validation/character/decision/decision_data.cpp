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

static bool class_feature_is_in(const dnd::Feature& feature_to_find, const std::vector<dnd::ClassFeature>& features) {
    for (const dnd::ClassFeature& feature : features) {
        if (&feature == &feature_to_find) {
            return true;
        }
    }
    return false;
}

static bool feature_is_in(const dnd::Feature& feature_to_find, const std::vector<dnd::Feature>& features) {
    for (const dnd::Feature& feature : features) {
        if (&feature == &feature_to_find) {
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

    // TODO: This needs to be redesigned.
    // Currently, there is no way to link a decision to a higher level part of a class feature
    // or any choosable that might have a choice.
    // Idea: add a new library to the content for class features and search in each of the three libraries
    const Feature& linked_feature = content.get_features().get(feature_name).value();
    const Effects& effects = linked_feature.get_main_effects();
    bool target_found_in_feature = &effects == target;
    if (!target_found_in_feature) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, "Decision cannot be linked to any of the feature's parts."
        );
    }

    bool feature_found = false;
    if (character_data != nullptr) {
        const std::string& race_name = character_data->character_basis_data.race_name;
        auto race_optional = content.get_character_races().get(race_name);
        if (race_optional.has_value()) {
            const CharacterRace& race = race_optional.value();
            feature_found = feature_is_in(linked_feature, race.get_features());
        }
        const std::string& subrace_name = character_data->character_basis_data.subrace_name;
        auto subrace_optional = content.get_character_subraces().get(subrace_name);
        if (!feature_found && subrace_optional.has_value()) {
            const CharacterSubrace& subrace = subrace_optional.value();
            feature_found = feature_is_in(linked_feature, subrace.get_features());
        }
        const std::string& class_name = character_data->character_basis_data.class_name;
        auto class_optional = content.get_character_classes().get(class_name);
        if (!feature_found && class_optional.has_value()) {
            const CharacterClass& cls = class_optional.value();
            feature_found = class_feature_is_in(linked_feature, cls.get_features());
        }
        const std::string& subclass_name = character_data->character_basis_data.subclass_name;
        auto subclass_optional = content.get_character_subclasses().get(subclass_name);
        if (!feature_found && content.get_character_subclasses().contains(subclass_name)) {
            const CharacterSubclass& subclass = subclass_optional.value();
            feature_found = class_feature_is_in(linked_feature, subclass.get_features());
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

const dnd::Effects* dnd::DecisionData::get_target() const noexcept { return target; }

void dnd::DecisionData::set_target(const dnd::Effects* new_target) noexcept { target = new_target; }
