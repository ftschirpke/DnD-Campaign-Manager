#ifndef CHARACTER_DECISION_HPP_
#define CHARACTER_DECISION_HPP_

#include "dnd_config.hpp"

#include "models/effect_holder/choice.hpp"
#include "models/effect_holder/effect_holder.hpp"

namespace dnd {

class CharacterDecision {
public:
    CharacterDecision(const Choice* const choice) noexcept;
    const Choice* const choice;
    EffectHolder decision_effects;
    EffectHolder createEffectHolder() const;
};

inline CharacterDecision::CharacterDecision(const Choice* const choice) noexcept : choice(choice) {}

} // namespace dnd

#endif // CHARACTER_DECISION_HPP_
