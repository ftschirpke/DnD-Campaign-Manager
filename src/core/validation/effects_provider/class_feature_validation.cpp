#include <dnd_config.hpp>

#include "class_feature_validation.hpp"

#include <optional>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/validation/effects/effects_validation.hpp>
#include <core/validation/effects_provider/feature_validation.hpp>

namespace dnd {

static std::optional<ValidationError> validate_level(const ClassFeature::Data& feature_validation) {
    if (feature_validation.level < 0) {
        return ValidationError(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Feature level must be positive.");
    } else if (feature_validation.level > 20) {
        return ValidationError(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Feature level must be at most 20.");
    }
    return std::nullopt;
}

Errors validate_class_feature_nonrecursively(const ClassFeature::Data& data) {
    Errors errors = validate_feature_nonrecursively(data);
    if (data.class_name.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Class feature's class name must not be empty"
        );
    }
    if (data.class_source_name.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Class feature's class source name must not be empty"
        );
    }
    std::optional<ValidationError> level_error = validate_level(data);
    if (level_error.has_value()) {
        errors.add_validation_error(std::move(level_error.value()));
    }
    return errors;
}

Errors validate_class_feature_recursively_for_content(const ClassFeature::Data& data, const Content& content) {
    Errors errors = validate_feature_recursively_for_content(data, content);
    std::optional<ValidationError> level_error = validate_level(data);
    if (level_error.has_value()) {
        errors.add_validation_error(std::move(level_error.value()));
    }
    for (const auto& [_, effects_data] : data.higher_level_effects_data) {
        errors += validate_effects_recursively_for_content(effects_data, content);
    }
    return errors;
}

} // namespace dnd
