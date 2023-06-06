#include <dnd_config.hpp>

#include "character.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/basic_mechanics/abilities.hpp>
#include <core/models/character_class.hpp>
#include <core/models/character_race.hpp>
#include <core/models/character_state.hpp>
#include <core/models/effect_holder/feature.hpp>

constexpr std::array<int, 20> dnd::Character::minxp_for_level = {
    0,     300,    900,    2700,   6500,   14000,  23000,  34000,  48000,  64000,
    85000, 100000, 120000, 140000, 165000, 195000, 225000, 265000, 305000, 355000,
};

int dnd::Character::level_for_xp(int xp) {
    if (xp < 0) {
        throw std::invalid_argument("XP value cannot be negative.");
    }
    for (size_t lv = 0; lv < 20; ++lv) {
        if (minxp_for_level[lv] > xp) {
            return static_cast<int>(lv);
        }
    }
    return 20;
}

const std::unordered_map<std::string, int> dnd::Character::get_constants() const {
    std::unordered_map<std::string, int> character_constants = {
        {"LEVEL", get_level() * 100},
        {"CLASS_LEVEL", get_level() * 100},
        {"XP", get_xp() * 100},
        {"ARMOR_ON", false},        // TODO
        {"HAS_SPELLCASTING", false} // TODO
    };
    return character_constants;
}

const std::unordered_map<std::string, int> dnd::Character::get_initial_attribute_values() const {
    std::unordered_map<std::string, int> character_initial_values = {
        {"MAXHP", 0},
    };
    for (size_t i = 0; i < 6; ++i) {
        const std::string ability_str(ability_cstrings_inorder[i]);
        character_initial_values.emplace(ability_str, base_ability_scores[i] * 100);
        character_initial_values.emplace(ability_str + "MAX", 2000);
    }
    return character_initial_values;
}

void dnd::Character::determine_state() {
    DND_MEASURE_FUNCTION();

    state.reset(get_constants(), get_initial_attribute_values());

    state.addFeatureHolder(this);
    state.addFeatureHolder(class_ptr);
    state.addFeatureHolder(subclass_ptr);
    state.addFeatureHolder(race_ptr);
    state.addFeatureHolder(subrace_ptr);

    state.calculate();
}

std::vector<const dnd::Feature*> dnd::Character::all_features() const {
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
