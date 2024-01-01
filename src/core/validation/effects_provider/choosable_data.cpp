#include <dnd_config.hpp>

#include "choosable_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

static Errors validate_choosable_type(const ChoosableData& data) {
    Errors errors;
    if (data.type.empty()) {
        errors.add_validation_error(ValidationError::Code::MISSING_ATTRIBUTE, "Choosable Feature has empty type");
    }
    return errors;
}

Errors validate_choosable_nonrecursively(const ChoosableData& data) {
    Errors errors = validate_feature_nonrecursively(data);
    errors += validate_choosable_type(data);
    return errors;
}

Errors validate_choosable_recursively_for_content(const ChoosableData& data, const Content& content) {
    Errors errors = validate_feature_recursively_for_content(data, content);
    errors += validate_choosable_type(data);
    for (const ConditionData& prerequisite_data : data.prerequisites_data) {
        errors += validate_condition(prerequisite_data);
    }
    return errors;
}

} // namespace dnd
