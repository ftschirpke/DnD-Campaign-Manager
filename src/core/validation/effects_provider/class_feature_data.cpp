#include <dnd_config.hpp>

#include "class_feature_data.hpp"

#include <memory>
#include <optional>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

ClassFeatureData::ClassFeatureData(const ValidationData* parent) noexcept
    : FeatureData(parent), level(1), higher_level_effects_data() {}

std::unique_ptr<ValidationData> ClassFeatureData::pack() const { return std::make_unique<ClassFeatureData>(*this); }

static std::optional<ValidationError> validate_level(const ClassFeatureData& feature_data) {
    if (feature_data.level <= 0) {
        return ValidationError(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, feature_data.get_parent(), "Feature level must be positive."
        );
    } else if (feature_data.level > 20) {
        return ValidationError(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, feature_data.get_parent(),
            "Feature level must be at most 20."
        );
    }
    return std::nullopt;
}

Errors ClassFeatureData::validate() const {
    Errors errors = FeatureData::validate();
    std::optional<ValidationError> level_error = validate_level(*this);
    if (level_error.has_value()) {
        errors.add_validation_error(std::move(level_error.value()));
    }
    for (const auto& [_, effects_data] : higher_level_effects_data) {
        errors += effects_data.validate();
    }
    return errors;
}

Errors ClassFeatureData::validate_nonrecursively() const {
    Errors errors = FeatureData::validate_nonrecursively();
    std::optional<ValidationError> level_error = validate_level(*this);
    if (level_error.has_value()) {
        errors.add_validation_error(std::move(level_error.value()));
    }
    return errors;
}

Errors ClassFeatureData::validate_relations(const Content& content) const {
    Errors errors = main_effects_data.validate_relations(content);
    for (const auto& [_, effects_data] : higher_level_effects_data) {
        errors += effects_data.validate_relations(content);
    }
    return errors;
}

} // namespace dnd
