#include <dnd_config.hpp>

#include "character_progression.hpp"

#include <array>
#include <stdexcept>

namespace dnd {

static constexpr std::array<int, 20> minxp_for_level = {
    0,     300,    900,    2700,   6500,   14000,  23000,  34000,  48000,  64000,
    85000, 100000, 120000, 140000, 165000, 195000, 225000, 265000, 305000, 355000,
};

int level_to_xp(int level) {
    if (level < 1 || level > 20) {
        throw std::invalid_argument("Level must be between 1 and 20 (inclusive).");
    }
    return minxp_for_level[static_cast<size_t>(level) - 1];
}

int xp_to_level(int xp) {
    if (xp < 0) {
        throw std::invalid_argument("XP value cannot be negative.");
    }
    for (size_t lv = 1; lv < 20; ++lv) {
        if (minxp_for_level[lv] > xp) {
            return static_cast<int>(lv);
        }
    }
    return 20;
}

// proficiency bonuses for levels 1-20
// 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6
int proficiency_bonus_for_level(int level) {
    if (level < 1 || level > 20) {
        throw std::invalid_argument("Level must be between 1 and 20 (inclusive).");
    }
    return 2 + (level - 1) / 4;
}

} // namespace dnd
