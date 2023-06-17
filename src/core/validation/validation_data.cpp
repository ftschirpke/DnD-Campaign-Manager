#include <dnd_config.hpp>

#include "validation_data.hpp"

#include <filesystem>
#include <string>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>

dnd::Errors dnd::ValidationData::validate() const {
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

dnd::Errors dnd::ValidationData::validate_relations(const ContentHolder& content) const {
    DND_UNUSED(content);
    return Errors();
}
