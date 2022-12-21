#include "character.hpp"

void dnd::Character::updateLevel() {
    if (xp_for_level.at(level) <= xp && (level == 20 || xp_for_level.at(level+1) > xp)) {
        return;
    }
    for (const auto& [lv, min_xp] : xp_for_level) {
        if (min_xp <= xp) {
            level = lv;
            return;
        }
    }
}
