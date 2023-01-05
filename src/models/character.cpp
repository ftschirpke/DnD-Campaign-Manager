#include "dnd_config.hpp"

#include "character.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/creature.hpp"
#include "models/creature_state.hpp"
#include "models/features/feature.hpp"

int dnd::Character::levelForXP(int xp) {
    if (xp < 0) {
        throw std::invalid_argument("XP cannot be negative.");
    }
    for (const auto [lv, min_xp] : xp_for_level) {
        if (min_xp > xp) {
            return lv - 1;
        }
    }
    return 20;
}

const std::unordered_map<std::string, int> dnd::Character::getConstants() const {
    const std::unordered_map<std::string, int> creature_constants = Creature::getConstants();
    std::unordered_map<std::string, int> character_constants = {
        {"LEVEL", 1},
        {"XP", 230},
    };
    // TODO: removed fixed values and multiply by 100 (except booleans)
    character_constants.insert(creature_constants.cbegin(), creature_constants.cend());
    return character_constants;
}

const std::unordered_map<std::string, int> dnd::Character::getInitialAttributeValues() const {
    const std::unordered_map<std::string, int> creature_initial_values = Creature::getInitialAttributeValues();
    std::unordered_map<std::string, int> character_initial_values = {
        {"ARMOR_ON", false},
    };
    // TODO: removed fixed values and multiply by 100 (except booleans)
    character_initial_values.insert(creature_initial_values.cbegin(), creature_initial_values.cend());
    return character_initial_values;
}

void dnd::Character::determineState() {
    state.reset(getConstants(), getInitialAttributeValues());

    state.addFeatureHolder(this);
    state.addFeatureHolder(class_ptr);
    state.addFeatureHolder(subclass_ptr);
    state.addFeatureHolder(race_ptr);
    state.addFeatureHolder(subrace_ptr);

    state.calculate();
}

std::vector<std::shared_ptr<const dnd::Feature>> dnd::Character::allFeatures() const {
    std::vector<std::shared_ptr<const dnd::Feature>> all_features = class_ptr->features;
    if (subclass_ptr != nullptr) {
        all_features.insert(all_features.end(), subclass_ptr->features.cbegin(), subclass_ptr->features.cend());
    }
    all_features.insert(all_features.end(), race_ptr->features.cbegin(), race_ptr->features.cend());
    if (subrace_ptr != nullptr) {
        all_features.insert(all_features.end(), subrace_ptr->features.cbegin(), subrace_ptr->features.cend());
    }
    return all_features;
}
