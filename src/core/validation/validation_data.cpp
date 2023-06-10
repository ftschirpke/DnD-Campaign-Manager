#include <dnd_config.hpp>

#include "validation_data.hpp"

#include <filesystem>
#include <string>

#include <core/errors/errors.hpp>

dnd::Errors dnd::ValidationData::validate() {
    Errors errors;
    if (name.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Name is empty");
    }
    if (description.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Description is empty");
    }
    if (!std::filesystem::exists(source_path)) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Source path does not exist");
    }
    return errors;
}
