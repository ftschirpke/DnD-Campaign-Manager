#ifndef CHARACTER_STATE_HPP_
#define CHARACTER_STATE_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/models/effect_holder/action_holder.hpp>
#include <core/models/effect_holder/character_decision.hpp>
#include <core/models/effect_holder/effect_holder.hpp>
#include <core/models/effect_holder/proficiency_holder.hpp>
#include <core/models/effect_holder/riv_holder.hpp>
#include <core/models/feature_holder.hpp>

namespace dnd {

/**
 * @brief A class representing the current state of a character
 */
class CharacterState {
public:
    /**
     * @brief Determines the ability modifier corresponding to a given ability score
     * @param ability_score the ability score between 1 and 30
     * @return the corresponding ability modifier between -5 and +10
     */
    static int modifier(int ability_score) noexcept;
    /**
     * @brief Constructs a CharacterState
     * @param decisions the decisions of the character
     */
    CharacterState(const std::vector<CharacterDecision>& decisions) noexcept;
    /**
     * @brief Constructs a CharacterState with its constants and initial attributes for calculation
     * @param constants all the values that may be used but not changed during calculation
     * @param initial_attributes all the values that may be used and changed during calculation
     * @param decisions the decisions of the character
     */
    CharacterState(
        const std::unordered_map<std::string, int>& constants,
        const std::unordered_map<std::string, int>& initial_attributes, const std::vector<CharacterDecision>& decisions
    ) noexcept;
    /**
     * @brief Sets the constants and attributes to the given values, so that a recalculation of the state can start
     * @param new_constants the new constants (values that may be used but not changed during calculation)
     * @param new_initial_attributes the new initial attributes (values that may be used and changed during calculation)
     */
    void reset(
        const std::unordered_map<std::string, int>& new_constants,
        const std::unordered_map<std::string, int>& new_initial_attributes
    ) noexcept;
    /**
     * @brief Add a character's feature holder to the state so that it can be used for calculation
     * @param feature_holder_ptr a pointer to the feature holder
     */
    void add_feature_holder(const FeatureHolder* const feature_holder_ptr);
    /**
     * @brief Determines the state for the currently set constants, intial values, and feature holders
     */
    void calculate();

    // values that may be used but not changed during calculation
    std::unordered_map<std::string, int> constants;
    // values that may be used and changed during calculation
    std::unordered_map<std::string, int> attributes;
    // all the effect holders that are currently affecting the character (only these are needed for the calculation)
    std::vector<const EffectHolder*> active_effect_holders;
    // the character decisions for features etc.
    const std::vector<CharacterDecision>& decisions;
    // the actions currently available to the character
    ActionHolder actions;
    // the proficiencies currently available to the character
    ProficiencyHolder proficiencies;
    // the resistances, immunities, and vulnerabilities currently available to the character
    RIVHolder rivs;
private:
    /**
     * @brief Add an effect holder without choices if it is currently affecting the character
     * @param effect_holder the effect holder to add
     * @return "true" if the effect holder was added, "false" otherwise
     */
    bool add_effect_holder(const dnd::EffectHolder& effect_holder);
    /**
     * @brief Add an effect holder with choices if it is currently affecting the character
     * @param effect_holder the effect holder to add
     * @return "true" if the effect holder was added, "false" otherwise
     */
    bool add_effect_holder_with_choices(const dnd::EffectHolderWithChoices& eh_with_choice);
    /**
     * @brief Apply all ability score effects to the ability score attributes
     * This usually should be done first because most attributes depend on ability modifiers and therefore,
     * they are required in advance
     */
    void apply_ability_score_effects();
    /**
     * @brief Apply all effect not affecting ability scores
     */
    void apply_normal_effects();
    /**
     * @brief Determine all the ability modifiers as well as modifiers for skills and saving throws
     * This should be done after applying the ability score effects,
     * otherwise some of the ability modifiers might be incorrect
     */
    void determine_modifiers();
};


} // namespace dnd

#endif // CHARACTER_STATE_HPP_
