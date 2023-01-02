#include "dnd_config.hpp"

#include "feature.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "models/features/activation.hpp"
#include "models/features/effect.hpp"

bool dnd::Feature::isActive(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    for (const auto& activation : activations) {
        if (!activation->check(attributes, constants)) {
            return false;
        }
    }
    return true;
}

// Do not call this function too often, unless you are sure the feature activation is only level-dependent.
bool dnd::Feature::isActiveForLevel(int level) const {
    std::unordered_map<std::string, int> empty_attributes;
    try {
        return isActive(empty_attributes, {{"CLASS_LEVEL", level * 100}});
    } catch (const std::out_of_range& e) {
        return false;
    }
}
