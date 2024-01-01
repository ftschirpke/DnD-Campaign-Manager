#include <dnd_config.hpp>

#include "progression_data.hpp"

#include <fmt/format.h>

#include <core/basic_mechanics/character_progression.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors validate_progression(const ProgressionData& data) {
    Errors errors;
    bool valid_level = data.level >= 1 && data.level <= 20;
    bool valid_xp = data.xp >= 0;
    if (!valid_level) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Character level ({}) must be between 1 and 20 (inclusive).", data.level)
        );
    }
    if (!valid_xp) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Character xp ({}) must be 0 or larger.", data.xp)
        );
    }
    if (valid_level && valid_xp && xp_to_level(data.xp) != data.level) {
        errors.add_validation_error(
            ValidationError::Code::INCONSISTENT_ATTRIBUTES,
            fmt::format("Character level ({}) and xp ({}) do not match.", data.level, data.xp)
        );
    }

    for (int hit_dice_roll : data.hit_dice_rolls) {
        if (hit_dice_roll < 1) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Character has invalid hit dice roll {} (must be 1 or larger).", hit_dice_roll)
            );
        }
    }

    if (static_cast<size_t>(data.level) != data.hit_dice_rolls.size()) {
        errors.add_validation_error(
            ValidationError::Code::INCONSISTENT_ATTRIBUTES,
            fmt::format(
                "Character level ({}) and hit dice rolls ({}) do not match.", data.level, data.hit_dice_rolls.size()
            )
        );
    }
    return errors;
}

} // namespace dnd
