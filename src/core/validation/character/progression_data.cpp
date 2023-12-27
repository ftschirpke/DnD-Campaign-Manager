#include <dnd_config.hpp>

#include "progression_data.hpp"

#include <fmt/format.h>

#include <core/basic_mechanics/character_progression.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

ProgressionData::ProgressionData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

Errors ProgressionData::validate() const {
    Errors errors;
    bool valid_level = level >= 1 && level <= 20;
    bool valid_xp = xp >= 0;
    if (!valid_level) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format("Character level ({}) must be between 1 and 20 (inclusive).", level)
        );
    }
    if (!valid_xp) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format("Character xp ({}) must be 0 or larger.", xp)
        );
    }
    if (valid_level && valid_xp && xp_to_level(xp) != level) {
        errors.add_validation_error(
            ValidationError::Code::INCONSISTENT_ATTRIBUTES, parent,
            fmt::format("Character level ({}) and xp ({}) do not match.", level, xp)
        );
    }

    for (int hit_dice_roll : hit_dice_rolls) {
        if (hit_dice_roll < 1) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format("Character has invalid hit dice roll {} (must be 1 or larger).", hit_dice_roll)
            );
        }
    }

    if (static_cast<size_t>(level) != hit_dice_rolls.size()) {
        errors.add_validation_error(
            ValidationError::Code::INCONSISTENT_ATTRIBUTES, parent,
            fmt::format("Character level ({}) and hit dice rolls ({}) do not match.", level, hit_dice_rolls.size())
        );
    }
    return errors;
}

} // namespace dnd
