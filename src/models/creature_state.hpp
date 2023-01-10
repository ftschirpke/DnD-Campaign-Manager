#ifndef CREATURE_STATE_HPP_
#define CREATURE_STATE_HPP_

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/effects_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

const std::vector<std::string> abilities = {
    "STR", "DEX", "CON", "INT", "WIS", "CHA",
};

const std::map<std::string, std::string> skills = {
    {"ACROBATICS", "DEX"},    {"ANIMAL_HANDLING", "WIS"}, {"ARCANA", "INT"},   {"ATHLETICS", "STR"},
    {"DECEPTION", "CHA"},     {"HISTORY", "INT"},         {"INSIGHT", "WIS"},  {"INTIMIDATION", "CHA"},
    {"INVESTIGATION", "INT"}, {"MEDICINE", "WIS"},        {"NATURE", "INT"},   {"PERCEPTION", "WIS"},
    {"PERFORMANCE", "CHA"},   {"PERSUASION", "CHA"},      {"RELIGION", "INT"}, {"SLEIGHT_OF_HAND", "DEX"},
    {"STEALTH", "DEX"},       {"SURVIVAL", "WIS"},
};

class CreatureState {
public:
    std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes;
    std::vector<const Feature*> active_features;
    ActionHolder actions;
    ProficiencyHolder proficiencies;
    RIVHolder rivs;
    CreatureState() noexcept = default;
    CreatureState(
        const std::unordered_map<std::string, int>& constants,
        const std::unordered_map<std::string, int>& initial_attributes
    ) noexcept;
    void reset(
        const std::unordered_map<std::string, int>& new_constants,
        const std::unordered_map<std::string, int>& new_initial_attributes
    ) noexcept;
    void addFeatureHolder(const FeatureHolder* const feature_holder_ptr);
    void calculate();
    static int modifier(int ability_score) noexcept;
    static bool isAbility(const std::string& attribute_name);
private:
    void applyAbilityScoreEffects();
    void applyNormalEffects();
    void determineModifiers();
};

inline CreatureState::CreatureState(
    const std::unordered_map<std::string, int>& constants,
    const std::unordered_map<std::string, int>& initial_attributes
) noexcept
    : constants(constants), attributes(initial_attributes) {}

inline void CreatureState::reset(
    const std::unordered_map<std::string, int>& new_constants,
    const std::unordered_map<std::string, int>& new_initial_attributes
) noexcept {
    constants = new_constants;
    attributes = new_initial_attributes;
}

inline int CreatureState::modifier(int ability_score) noexcept { return ability_score / 2 - 5; }

inline bool CreatureState::isAbility(const std::string& attribute_name) {
    return std::find(abilities.cbegin(), abilities.cend(), attribute_name) != abilities.cend();
}

} // namespace dnd

#endif // CREATURE_STATE_HPP_
