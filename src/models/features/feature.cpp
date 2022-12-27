#include "feature.hpp"

#include <memory>
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
