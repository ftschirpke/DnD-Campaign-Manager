#include <dnd_config.hpp>

#include "validation_data.hpp"

#include <filesystem>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>

namespace dnd {

Errors validate_name_description_and_source(const ValidationData& data) {
    Errors errors;
    if (data.name.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(), "Name is empty");
    }
    if (data.description.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
            fmt::format("Description for '{}' is empty", data.name)
        );
    }
    if (!std::filesystem::exists(data.source_path)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
            fmt::format("Source path '{}' for '{}' does not exist", data.source_path.string(), data.name)
        );
    }
    return errors;
}

} // namespace dnd
