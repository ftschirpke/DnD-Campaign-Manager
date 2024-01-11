#include <dnd_config.hpp>

#include "character_progression.hpp"

#include <array>

namespace dnd {

static constexpr std::array<int, 20> minxp_for_level = {
    0,     300,    900,    2700,   6500,   14000,  23000,  34000,  48000,  64000,
    85000, 100000, 120000, 140000, 165000, 195000, 225000, 265000, 305000, 355000,
};

tl::expected<int, RuntimeError> xp_for_level(int level) {
    if (level < 1 || level > 20) {
        return tl::unexpected(
            RuntimeError(RuntimeError::Code::INVALID_ARGUMENT, "Level must be between 1 and 20 (inclusive).")
        );
    }
    return minxp_for_level[static_cast<size_t>(level) - 1];
}

tl::expected<int, RuntimeError> level_for_xp(int xp) {
    if (xp < 0) {
        return tl::unexpected(RuntimeError(RuntimeError::Code::INVALID_ARGUMENT, "XP value cannot be negative."));
    }
    for (size_t lv = 1; lv < 20; ++lv) {
        if (minxp_for_level[lv] > xp) {
            return static_cast<int>(lv);
        }
    }
    return 20;
}

// proficiency bonuses for levels 1-20
// levels 1-4: +2
// levels 5-8: +3
// levels 9-12: +4
// levels 13-16: +5
// levels 17-20: +6
tl::expected<int, RuntimeError> proficiency_bonus_for_level(int level) {
    if (level < 1 || level > 20) {
        return tl::unexpected(
            RuntimeError(RuntimeError::Code::INVALID_ARGUMENT, "Level must be between 1 and 20 (inclusive).")
        );
    }
    return 2 + (level - 1) / 4;
}

} // namespace dnd
