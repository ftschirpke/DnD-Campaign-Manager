#include "dnd_config.hpp"

#include "effects_holder.hpp"

bool dnd::EffectsHolder::isActive(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    for (const auto& activation : activations) {
        if (!activation->check(attributes, constants)) {
            return false;
        }
    }
    return true;
}
