#include <dnd_config.hpp>

#include "validation_error.hpp"

#include <string>

#include <core/validation/validation_data.hpp>

namespace dnd {

ValidationError::ValidationError(ValidationError::Code error_code, const std::string& message)
    : error_code(error_code), error_message(message) {}

ValidationError::Code ValidationError::get_error_code() const { return error_code; }

const std::string& ValidationError::get_error_message() const { return error_message; }

} // namespace dnd
