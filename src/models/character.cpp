#include "dnd_config.hpp"

#include "character.hpp"

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "basic_mechanics/abilities.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_state.hpp"
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
    std::unordered_map<std::string, int> character_constants = {
        {"LEVEL", getLevel() * 100},
        {"CLASS_LEVEL", getLevel() * 100},
        {"XP", getXP() * 100},
    };
    return character_constants;
}

const std::unordered_map<std::string, int> dnd::Character::getInitialAttributeValues() const {
    std::unordered_map<std::string, int> character_initial_values = {
        {"MAXHP", 0},
        {"ARMOR_ON", false},
    };
    for (size_t i = 0; i < 6; ++i) {
        character_initial_values.emplace(ability_strings_inorder[i], base_ability_scores[i] * 100);
        character_initial_values.emplace(ability_strings_inorder[i] + "MAX", 2000);
    }
    return character_initial_values;
}

void dnd::Character::determineState() {
    DND_MEASURE_FUNCTION();

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
