#include <dnd_config.hpp>

#include "choosable_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

ChoosableData::ChoosableData() noexcept : FeatureData(this) {}

std::unique_ptr<ValidationData> ChoosableData::pack() const { return std::make_unique<ChoosableData>(*this); }

Errors ChoosableData::validate() const {
    Errors errors = FeatureData::validate();
    if (type.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Choosable Feature has empty type"
        );
    }
    for (const ConditionData& prerequisite_data : prerequisites_data) {
        errors += prerequisite_data.validate();
    }
    return errors;
}

} // namespace dnd
