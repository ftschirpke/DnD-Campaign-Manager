#include <dnd_config.hpp>

#include "decision_validation.hpp"

#include <algorithm>
#include <cassert>
#include <map>
#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/validation/character/character_validation.hpp>
#include <core/validation/effects/effects_validation.hpp>
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

Errors validate_decision_for_content(const Decision::Data& data, const Content& content) {
    Errors errors = validate_decision_raw(data);
    errors += validate_decision_relations(data, content);
    return errors;
}

} // namespace dnd
