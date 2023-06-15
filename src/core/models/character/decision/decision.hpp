#ifndef DECISION_HPP_
#define DECISION_HPP_

#include <dnd_config.hpp>

#include <core/models/effect_holder/choice/choice.hpp>
#include <core/models/effect_holder/effect_holder.hpp>

namespace dnd {

/**
 * @brief A class that represents a character's decision for a certain choice required or provided by a feature
 */
class Decision {
public:
private:
    Decision(const Choice* choice, EffectHolder effects) noexcept;

    const Choice* choice;
    EffectHolder effects;
};

} // namespace dnd

#endif // DECISION_HPP_
