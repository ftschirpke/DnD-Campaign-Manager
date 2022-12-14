#include "dnd_config.hpp"

#include "creature.hpp"

#include <string>
#include <unordered_map>

#include "models/creature_state.hpp"
#include "models/feature_holder.hpp"

const std::unordered_map<std::string, int> dnd::Creature::getConstants() const { return {}; }

const std::unordered_map<std::string, int> dnd::Creature::getInitialAttributeValues() const {
    std::unordered_map<std::string, int> initial_values = {
        {"MAXHP", 0},
    };
    for (int i = 0; i < 6; ++i) {
        initial_values.emplace(abilities[i], base_ability_scores[i]);
    }
    return initial_values;
}
