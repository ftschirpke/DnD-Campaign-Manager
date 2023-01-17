#include "dnd_config.hpp"

#include "feature.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>

// Do not call this function too often, unless you are sure the feature activation is only level-dependent.
bool dnd::Feature::isActiveForLevel(int level) const {
    std::unordered_map<std::string, int> empty_attributes;
    try {
        return isActive(empty_attributes, {{"CLASS_LEVEL", level * 100}});
    } catch (const std::out_of_range& e) {
        return false;
    }
}
