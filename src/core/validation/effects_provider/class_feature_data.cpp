#include <dnd_config.hpp>

#include "class_feature_data.hpp"

#include <optional>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

static std::optional<ValidationError> validate_level(const ClassFeature::Data& feature_data) {
    if (feature_data.level <= 0) {
        return ValidationError(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Feature level must be positive.");
    } else if (feature_data.level > 20) {
        return ValidationError(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Feature level must be at most 20.");
    }
    return std::nullopt;
}

Errors validate_class_feature_nonrecursively(const ClassFeature::Data& data) {
    Errors errors = validate_feature_nonrecursively(data);
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
