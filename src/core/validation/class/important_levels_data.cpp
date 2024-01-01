#include <dnd_config.hpp>

#include "important_levels_data.hpp"

#include <algorithm>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

ImportantLevelsData::ImportantLevelsData(std::shared_ptr<const ValidationData> parent) noexcept
    : ValidationSubdata(parent) {}

Errors validate_important_levels(const ImportantLevelsData& data) {
    Errors errors;
    if (data.feat_levels.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
            "Character class has no ability score improvement levels."
        );
        return errors;
    }
    if (*data.feat_levels.begin() <= 0 || *(--data.feat_levels.end()) > 20) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
            "Feat levels must all be between 1 and 20 (inclusive)."
        );
    }
    return errors;
}

} // namespace dnd
