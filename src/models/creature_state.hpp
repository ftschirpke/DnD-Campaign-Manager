#ifndef CREATURE_STATE_HPP_
#define CREATURE_STATE_HPP_

#include "dnd_config.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/effect_holder/action_holder.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/proficiency_holder.hpp"
#include "models/effect_holder/riv_holder.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class CreatureState {
public:
    std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes;
    std::vector<const EffectHolder*> active_effect_holders;
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


} // namespace dnd

#endif // CREATURE_STATE_HPP_
