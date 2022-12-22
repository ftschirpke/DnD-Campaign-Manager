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
    character_constants.insert(creature_constants.cbegin(), creature_constants.cend());
    return character_constants;
}

const std::unordered_map<std::string, int> dnd::Character::getInitialAttributeValues() const {
    const std::unordered_map<std::string, int> creature_initial_values = Creature::getInitialAttributeValues();
    std::unordered_map<std::string, int> character_initial_values = {
        {"ARMOR_ON", false},
    };
    character_initial_values.insert(creature_initial_values.cbegin(), creature_initial_values.cend());
    return character_initial_values;
}

void dnd::Character::determineState() {
    state.reset(getConstants(), getInitialAttributeValues());

    state.applyAbilityScoreFeatures(class_ptr);
    if (subclass_ptr != nullptr) {
        state.applyAbilityScoreFeatures(subclass_ptr);
    }
    state.applyAbilityScoreFeatures(race_ptr);
    if (subrace_ptr != nullptr) {
        state.applyAbilityScoreFeatures(subrace_ptr);
    }

    state.determineModifiers();

    // TODO: are proficiencies normal features or should they have their own function?

    state.applyNormalFeatures(class_ptr);
    if (subclass_ptr != nullptr) {
        state.applyNormalFeatures(subclass_ptr);
    }
    state.applyNormalFeatures(race_ptr);
    if (subrace_ptr != nullptr) {
        state.applyNormalFeatures(subrace_ptr);
    }
}
