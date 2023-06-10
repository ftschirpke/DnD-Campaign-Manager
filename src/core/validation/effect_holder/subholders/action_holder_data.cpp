#include <dnd_config.hpp>

#include "action_holder_data.hpp"

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::ActionHolderData::ActionHolderData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::ActionHolderData::validate() const {
    Errors errors;
    for (const auto& [action_name, action_description] : actions) {
        if (action_name.empty()) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Action name cannot be empty"
            );
        }
        if (action_description.empty()) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Action description cannot be empty"
            );
        }
    }
    return errors;
}
