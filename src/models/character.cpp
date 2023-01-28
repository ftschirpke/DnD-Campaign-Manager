#include "dnd_config.hpp"

#include "character.hpp"

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_state.hpp"
#include "models/creature.hpp"
#include "models/effect_holder/feature.hpp"

unsigned int dnd::Character::levelForXP(unsigned int xp) {
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
        {"MAXHP", 0},
        {"ARMOR_ON", false},
    };
    for (int i = 0; i < 6; ++i) {
        initial_values.emplace(ability_strings_inorder[i], base_ability_scores[i]);
    }
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

std::vector<const dnd::Feature*> dnd::Character::allFeatures() const {
    std::vector<const Feature*> all_features;

    for (const auto& feature : class_ptr->features) {
        all_features.emplace_back(&feature);
    }

    if (subclass_ptr != nullptr) {
        for (const auto& feature : subclass_ptr->features) {
            all_features.emplace_back(&feature);
        }
    }

    for (const auto& feature : race_ptr->features) {
        all_features.emplace_back(&feature);
    }

    if (subrace_ptr != nullptr) {
        for (const auto& feature : subrace_ptr->features) {
            all_features.emplace_back(&feature);
        }
    }
    return all_features;
}
