#include <dnd_config.hpp>

#include "progression.hpp"

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/character/progression_data.hpp>

namespace dnd {

Progression Progression::create_for(Data&& data) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create Progression object from invalid data.");
    }
    return Progression(data.level, data.xp, std::move(data.hit_dice_rolls));
}

int Progression::get_level() const noexcept { return level; }

int Progression::get_xp() const noexcept { return xp; }

const std::vector<int>& Progression::get_hit_dice_rolls() const noexcept { return hit_dice_rolls; }

Progression::Progression(int level, int xp, std::vector<int>&& hit_dice_rolls) noexcept
    : level(level), xp(xp), hit_dice_rolls(std::move(hit_dice_rolls)) {}

} // namespace dnd
