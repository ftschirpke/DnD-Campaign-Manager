#include "dnd_config.hpp"

#include "effect_holder.hpp"

#include <string>
#include <unordered_map>

#include "models/effect_holder/activation.hpp"

bool dnd::EffectHolder::isActive(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    for (auto activation_it = activations.cbegin(); activation_it != activations.cend(); ++activation_it) {
        if (!(*activation_it)->check(attributes, constants)) {
            return false;
        }
    }
    return true;
}