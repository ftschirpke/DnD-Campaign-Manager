#ifndef CHARACTER_STATE_HPP_
#define CHARACTER_STATE_HPP_

#include "dnd_config.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/effect_holder/action_holder.hpp"
#include "models/effect_holder/character_decision.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/proficiency_holder.hpp"
#include "models/effect_holder/riv_holder.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class CharacterState {
public:
    std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes;
    std::vector<const EffectHolder*> active_effect_holders;
    const std::vector<CharacterDecision>& decisions;
    ActionHolder actions;
    ProficiencyHolder proficiencies;
    RIVHolder rivs;
    CharacterState(const std::vector<CharacterDecision>& decisions) noexcept;
    CharacterState(
        const std::unordered_map<std::string, int>& constants,
        const std::unordered_map<std::string, int>& initial_attributes, const std::vector<CharacterDecision>& decisions
    ) noexcept;
    void reset(
        const std::unordered_map<std::string, int>& new_constants,
        const std::unordered_map<std::string, int>& new_initial_attributes
    ) noexcept;
    void addFeatureHolder(const FeatureHolder* const feature_holder_ptr);
    void calculate();
    static int modifier(int ability_score) noexcept;
private:
    bool addEffectHolder(const dnd::EffectHolder& effect_holder);
    bool addEffectHolderWithChoices(const dnd::EffectHolderWithChoices& eh_with_choice);
    void applyAbilityScoreEffects();
    void applyNormalEffects();
    void determineModifiers();
};

inline CharacterState::CharacterState(const std::vector<CharacterDecision>& decisions) noexcept
    : decisions(decisions) {}

inline CharacterState::CharacterState(
    const std::unordered_map<std::string, int>& constants,
    const std::unordered_map<std::string, int>& initial_attributes, const std::vector<CharacterDecision>& decisions
) noexcept
    : constants(constants), attributes(initial_attributes), decisions(decisions) {}

inline void CharacterState::reset(
    const std::unordered_map<std::string, int>& new_constants,
    const std::unordered_map<std::string, int>& new_initial_attributes
) noexcept {
    constants = new_constants;
    attributes = new_initial_attributes;
}

inline int CharacterState::modifier(int ability_score) noexcept { return ability_score / 2 - 5; }


} // namespace dnd

#endif // CHARACTER_STATE_HPP_
