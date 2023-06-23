#include <dnd_config.hpp>

#include "important_levels_data.hpp"

#include <vector>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/character_class/character_class_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::ImportantLevelsData::ImportantLevelsData(const CharacterClassData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::ImportantLevelsData::validate() const {
    Errors errors;
    for (auto asi_level : asi_levels) {
        if (asi_level <= 0 || asi_level > 20) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                "ASI (ability score improvement) levels must all be between 1 and 20 (inclusive)."
            );
            break;
        }
    }
    if (asi_levels.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            "Character class has no ability score improvement levels."
        );
    }
    return errors;
}
