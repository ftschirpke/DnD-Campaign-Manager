#include "character.hpp"

#include <stdexcept>

int dnd::Character::levelForXP(int xp) {
    if (xp < 0) {
        throw std::invalid_argument("XP cannot be negative.");
    }
    for (const auto& [level, min_xp] : xp_for_level) {
        if (min_xp <= xp) {
            return level;
        }
    }
    return 0; // should never happen
}
