#ifndef EFFECT_HOLDER_HPP_
#define EFFECT_HOLDER_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>

#include "models/effect_holder/action_holder.hpp"
#include "models/effect_holder/activation.hpp"
#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/extra_spells_holder.hpp"
#include "models/effect_holder/proficiency_holder.hpp"
#include "models/effect_holder/riv_holder.hpp"

namespace dnd {

/**
 * @brief A class representing a part of a feature or objects alike that provides certain effects to a character
 */
class EffectHolder {
public:
    EffectHolder() noexcept = default;
    EffectHolder(EffectHolder&& other) noexcept = default;
    EffectHolder& operator=(EffectHolder&& other) noexcept = default;
    /**
     * @brief Returns true if the EffectHolder is empty
     * @return "true" if the EffectHolder is empty, "false" otherwise
     */
    bool empty() const;
    /**
     * @brief Checks whether the effect holder is active (provides its effects) for given attributes and constants
     * @param attributes the character attributes
     * @param constants the character constants
     * @return "true" if the effect holder is active for a character with these attributes and constants,
     * "false" otherwise
     */
    bool isActive(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;

    // TODO: should these really be public AND non-const?

    // a collection of conditions that need to be fulfilled for this effect holder to be active (provide its effects)
    std::vector<std::unique_ptr<Activation>> activations;
    // the actions the effect holder provides
    ActionHolder actions;
    // all the extra spells this effect holder allows you to cast or adds to your spell list etc.
    ExtraSpellsHolder extra_spells;
    // all the things this effect holder makes you proficient at
    ProficiencyHolder proficiencies;
    // all the resistances, immunities, and vulnerabilities this effect holder provides
    RIVHolder rivs;
    // all the ways this effect holder affects the calculation of ability-score-related attributes of a character
    std::unordered_map<EffectTime, std::vector<std::unique_ptr<const Effect>>> ability_score_effects;
    // all the ways this effect holder affects the calculation of non-ability-score-related attributes of a character
    std::unordered_map<EffectTime, std::vector<std::unique_ptr<const Effect>>> normal_effects;
};

inline bool EffectHolder::empty() const {
    return activations.empty() && actions.empty() && extra_spells.empty() && proficiencies.empty() && rivs.empty()
           && ability_score_effects.empty() && normal_effects.empty();
}

} // namespace dnd

#endif // EFFECT_HOLDER_HPP_
