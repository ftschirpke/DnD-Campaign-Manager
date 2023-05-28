#ifndef EFFECT_HOLDER_WITH_CHOICES_HPP_
#define EFFECT_HOLDER_WITH_CHOICES_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <vector>

#include "models/effect_holder/choice.hpp"
#include "models/effect_holder/effect_holder.hpp"

namespace dnd {

/**
 * @brief A class representing a part of a feature that provides certain effects to a character and
 * additionally requires the character to make choices
 */
class EffectHolderWithChoices : public EffectHolder {
public:
    EffectHolderWithChoices() noexcept = default;
    EffectHolderWithChoices(EffectHolderWithChoices&& other) noexcept = default;
    EffectHolderWithChoices& operator=(EffectHolderWithChoices&& other) noexcept = default;
    /**
     * @brief Returns true if the EffectHolderWithChoices is empty
     * @return "true" if the EffectHolderWithChoices is empty, "false" otherwise
     */
    bool empty() const;

    // the choices this effect holder needs the character to make
    std::vector<std::unique_ptr<Choice>> choices;
};

inline bool EffectHolderWithChoices::empty() const { return EffectHolder::empty() && choices.empty(); }

} // namespace dnd

#endif // EFFECT_HOLDER_WITH_CHOICES_HPP_
