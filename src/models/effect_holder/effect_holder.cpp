#include "dnd_config.hpp"

#include "effect_holder.hpp"

bool dnd::EffectHolder::isActive(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    for (const auto& activation : activations) {
        if (!activation->check(attributes, constants)) {
            return false;
        }
    }
    return true;
}
