#include <dnd_config.hpp>

#include "choosable_feature_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/effect_holder/condition/condition_data.hpp>
#include <core/validation/feature/feature_data.hpp>

dnd::ChoosableFeatureData::ChoosableFeatureData() noexcept : FeatureData(this) {}

std::unique_ptr<dnd::ValidationData> dnd::ChoosableFeatureData::pack() const {
    return std::make_unique<ChoosableFeatureData>(*this);
}

dnd::Errors dnd::ChoosableFeatureData::validate() const {
    Errors errors = FeatureData::validate();
    if (type.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Choosable Feature has empty type"
        );
    }
    for (const auto& prerequisite_data : prerequisites_data) {
        errors += prerequisite_data.validate();
    }
    return errors;
}
