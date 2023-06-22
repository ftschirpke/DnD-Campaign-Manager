#include <dnd_config.hpp>

#include "validation_error.hpp"

#include <memory>
#include <string>
#include <utility>

#include <core/validation/validation_data.hpp>

dnd::ValidationError::ValidationError(
    dnd::ValidationErrorCode error_code, const dnd::ValidationData* validation_data, const std::string& message
) noexcept
    : error_code(error_code), validation_data(std::make_unique<ValidationData>(*validation_data)),
      error_message(message) {}

dnd::ValidationErrorCode dnd::ValidationError::get_error_code() const noexcept { return error_code; }

const dnd::ValidationData* dnd::ValidationError::get_validation_data() const noexcept { return validation_data.get(); }

const std::string& dnd::ValidationError::get_error_message() const noexcept { return error_message; }
