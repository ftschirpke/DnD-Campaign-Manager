#include <dnd_config.hpp>

#include "important_levels_data.hpp"

#include <algorithm>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::ImportantLevelsData::ImportantLevelsData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::ImportantLevelsData::validate() const {
    Errors errors;
    if (feat_levels.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            "Character class has no ability score improvement levels."
        );
        return errors;
    }
    if (*feat_levels.begin() <= 0 || *(--feat_levels.end()) > 20) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            "Feat levels must all be between 1 and 20 (inclusive)."
        );
    }
    return errors;
}
