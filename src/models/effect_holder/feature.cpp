#include "dnd_config.hpp"

#include "feature.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "models/effect_holder/effect_holder.hpp"

// Do not call this function too often, unless you are sure the feature activation is only level-dependent.
bool dnd::Feature::isActiveForLevel(int level) const {
    std::unordered_map<std::string, int> empty_attributes;
    try {
        return main_part.isActive(empty_attributes, {{"CLASS_LEVEL", level * 100}});
    } catch (const std::out_of_range& e) {
        UNUSED(e);
        return false;
    }
}