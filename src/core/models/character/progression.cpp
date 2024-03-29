#include <dnd_config.hpp>

#include "progression.hpp"

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/character/progression_validation.hpp>

namespace dnd {

CreateResult<Progression> Progression::create(Data&& data) {
    Errors errors = validate_progression(data);
    if (!errors.ok()) {
        return InvalidCreate<Progression>(std::move(data), std::move(errors));
    }
    return ValidCreate(Progression(data.level, data.xp, std::move(data.hit_dice_rolls)));
}

int Progression::get_level() const { return level; }

int Progression::get_xp() const { return xp; }

const std::vector<int>& Progression::get_hit_dice_rolls() const { return hit_dice_rolls; }

Progression::Progression(int level, int xp, std::vector<int>&& hit_dice_rolls)
    : level(level), xp(xp), hit_dice_rolls(std::move(hit_dice_rolls)) {}

} // namespace dnd
