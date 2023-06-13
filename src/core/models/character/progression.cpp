#include <dnd_config.hpp>

#include "progression.hpp"

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/character/progression_data.hpp>

dnd::Progression dnd::Progression::create(dnd::ProgressionData&& data) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create Progression object from invalid data.");
    }
    return Progression(data.level, data.xp, std::move(data.hit_dice_rolls));
}

dnd::Progression::Progression(int level, int xp, std::vector<int>&& hit_dice_rolls) noexcept
    : level(level), xp(xp), hit_dice_rolls(std::move(hit_dice_rolls)) {}
