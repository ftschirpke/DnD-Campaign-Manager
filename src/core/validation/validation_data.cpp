#include <dnd_config.hpp>

#include "validation_data.hpp"

#include <filesystem>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>

dnd::Errors dnd::ValidationData::validate() const {
    Errors errors;
    if (name.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Name is empty");
    }
    if (description.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
            fmt::format("Description for '{}' is empty", name)
        );
    }
    if (!std::filesystem::exists(source_path)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
            fmt::format("Source path '{}' for '{}' does not exist", source_path.string(), name)
        );
    }
    return errors;
}

dnd::Errors dnd::ValidationData::validate_relations(const ContentHolder& content) const {
    DND_UNUSED(content);
    return Errors();
}
