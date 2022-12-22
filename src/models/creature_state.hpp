#ifndef CREATURE_STATE_HPP_
#define CREATURE_STATE_HPP_

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dnd {

// forward declaration
class FeatureHolder;

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
    CreatureState() = default;
    CreatureState(
        const std::unordered_map<std::string, int>& constants,
        const std::unordered_map<std::string, int>& initial_attributes
    );
    void reset(
        const std::unordered_map<std::string, int>& new_constants,
        const std::unordered_map<std::string, int>& new_initial_attributes
    );
    void applyAbilityScoreFeatures(std::shared_ptr<const FeatureHolder> feature_holder_ptr);
    void applyNormalFeatures(std::shared_ptr<const FeatureHolder> feature_holder_ptr);
    void determineModifiers();
    static int modifier(int ability_score);
    static bool isAbility(const std::string& attribute_name);
};

inline CreatureState::CreatureState(
    const std::unordered_map<std::string, int>& constants,
    const std::unordered_map<std::string, int>& initial_attributes
)
    : constants(constants), attributes(initial_attributes) {}

inline void CreatureState::reset(
    const std::unordered_map<std::string, int>& new_constants,
    const std::unordered_map<std::string, int>& new_initial_attributes
) {
    constants = new_constants;
    attributes = new_initial_attributes;
}

inline int CreatureState::modifier(int ability_score) { return ability_score / 2 - 5; }

inline bool CreatureState::isAbility(const std::string& attribute_name) {
    return std::find(abilities.cbegin(), abilities.cend(), attribute_name) != abilities.cend();
}

} // namespace dnd

#endif // CREATURE_STATE_HPP_
