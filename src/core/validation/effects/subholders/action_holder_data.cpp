#include <dnd_config.hpp>

#include "action_holder_data.hpp"

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

ActionHolderData::ActionHolderData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

Errors ActionHolderData::validate() const {
    Errors errors;
    for (const auto& [action_name, action_description] : actions) {
        if (action_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, "Action name cannot be empty"
            );
        }
        if (action_description.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, "Action description cannot be empty"
            );
        }
    }
    for (const auto& [bonus_action_name, bonus_action_description] : bonus_actions) {
        if (bonus_action_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, "Bonus action name cannot be empty"
            );
        }
        if (bonus_action_description.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, "Bonus action description cannot be empty"
            );
        }
    }
    for (const auto& [reaction_name, reaction_description] : reactions) {
        if (reaction_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, "Reaction name cannot be empty"
            );
        }
        if (reaction_description.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, "Reaction description cannot be empty"
            );
        }
    }
    return errors;
}

bool ActionHolderData::empty() const noexcept { return actions.empty() && bonus_actions.empty() && reactions.empty(); }

} // namespace dnd
