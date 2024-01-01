#include <dnd_config.hpp>

#include "validation_error.hpp"

#include <memory>
#include <string>
#include <utility>

#include <core/validation/validation_data.hpp>

namespace dnd {

ValidationError::ValidationError(
    ValidationError::Code error_code, std::shared_ptr<const ValidationData> validation_data, const std::string& message
) noexcept
    : error_code(error_code), validation_data(validation_data), error_message(message) {}

ValidationError::Code ValidationError::get_error_code() const noexcept { return error_code; }

const ValidationData* ValidationError::get_validation_data() const noexcept { return validation_data.get(); }

const std::string& ValidationError::get_error_message() const noexcept { return error_message; }

} // namespace dnd
