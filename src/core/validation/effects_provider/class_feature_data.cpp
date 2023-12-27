#include <dnd_config.hpp>

#include "class_feature_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

ClassFeatureData::ClassFeatureData(const ValidationData* parent) noexcept
    : FeatureData(parent), level(1), higher_level_effects_data() {}

std::unique_ptr<ValidationData> ClassFeatureData::pack() const { return std::make_unique<ClassFeatureData>(*this); }

Errors ClassFeatureData::validate() const {
    Errors errors = FeatureData::validate();
    if (level <= 0) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, get_parent(), "Feature level must be positive."
        );
    } else if (level > 20) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, get_parent(), "Feature level must be at most 20."
        );
    }
    for (const auto& [_, effects_data] : higher_level_effects_data) {
        errors += effects_data.validate();
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
