#include <dnd_config.hpp>

#include "action_holder_data.hpp"

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

ActionHolderData::ActionHolderData(std::shared_ptr<const ValidationData> parent) noexcept : ValidationSubdata(parent) {}

bool ActionHolderData::empty() const noexcept { return actions.empty() && bonus_actions.empty() && reactions.empty(); }

Errors validate_actions_holder(const ActionHolderData& data) {
    Errors errors;
    for (const auto& [action_name, action_description] : data.actions) {
        if (action_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(), "Action name cannot be empty"
            );
        }
        if (action_description.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(), "Action description cannot be empty"
            );
        }
    }
    for (const auto& [bonus_action_name, bonus_action_description] : data.bonus_actions) {
        if (bonus_action_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(), "Bonus action name cannot be empty"
            );
        }
        if (bonus_action_description.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
                "Bonus action description cannot be empty"
            );
        }
    }
    for (const auto& [reaction_name, reaction_description] : data.reactions) {
        if (reaction_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(), "Reaction name cannot be empty"
            );
        }
        if (reaction_description.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
                "Reaction description cannot be empty"
            );
        }
    }
    return errors;
}

} // namespace dnd
