#ifndef CHARACTER_DECISION_HPP_
#define CHARACTER_DECISION_HPP_

#include "dnd_config.hpp"

#include "core/models/effect_holder/choice.hpp"
#include "core/models/effect_holder/effect_holder.hpp"

namespace dnd {

/**
 * @brief A class that represents a character's decision for a certain choice required by a feature for example
 */
class CharacterDecision {
public:
    /**
     * @brief Constructs the character decision with the choice it refers to.
     * @param choice a pointer to the choice this decision is for
     */
    CharacterDecision(const Choice* const choice) noexcept;

    // the choice this decision was made for
    const Choice* const choice;
    // the effects of the decision
    EffectHolder decision_effects;
};

inline CharacterDecision::CharacterDecision(const Choice* const choice) noexcept : choice(choice) {}

} // namespace dnd

#endif // CHARACTER_DECISION_HPP_
