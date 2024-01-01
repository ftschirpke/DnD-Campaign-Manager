#include <dnd_config.hpp>

#include "important_levels_data.hpp"

#include <algorithm>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors validate_important_levels(const ImportantLevelsData& data) {
    Errors errors;
    if (data.feat_levels.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Character class has no ability score improvement levels."
        );
        return errors;
    }
    if (*data.feat_levels.begin() <= 0 || *(--data.feat_levels.end()) > 20) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Feat levels must all be between 1 and 20 (inclusive)."
        );
    }
    return errors;
}

} // namespace dnd
