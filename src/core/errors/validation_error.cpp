#include <dnd_config.hpp>

#include "validation_error.hpp"

#include <string>

dnd::ValidationError::ValidationError(
    dnd::ValidationErrorCode error_code, dnd::ValidationData* const validation_data, const std::string& message
) noexcept
    : error_code(error_code), validation_data(validation_data), message(message) {}

dnd::ValidationErrorCode dnd::ValidationError::get_error_code() const noexcept { return error_code; }

dnd::ValidationData* const dnd::ValidationError::get_validation_data() const noexcept { return validation_data; }

const std::string& dnd::ValidationError::get_error_message() const noexcept { return message; }
