#ifndef EFFECT_HOLDER_WITH_CHOICES_HPP_
#define EFFECT_HOLDER_WITH_CHOICES_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <vector>

#include "models/effect_holder/choice.hpp"
#include "models/effect_holder/effect_holder.hpp"

namespace dnd {

class EffectHolderWithChoices : public EffectHolder {
public:
    std::vector<std::unique_ptr<Choice>> choices;
    EffectHolderWithChoices() noexcept = default;
    EffectHolderWithChoices(EffectHolderWithChoices&& other) noexcept = default;
    EffectHolderWithChoices& operator=(EffectHolderWithChoices&& other) noexcept = default;
    bool empty() const;
};

inline bool EffectHolderWithChoices::empty() const { return EffectHolder::empty() && choices.empty(); }

} // namespace dnd

#endif // EFFECT_HOLDER_WITH_CHOICES_HPP_
