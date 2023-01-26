#ifndef EFFECT_HOLDER_WITH_CHOICES_HPP_
#define EFFECT_HOLDER_WITH_CHOICES_HPP_

#include <memory>
#include <vector>

#include "models/effect_holder/choice.hpp"
#include "models/effect_holder/effect_holder.hpp"

namespace dnd {

class EffectHolderWithChoices : public EffectHolder {
public:
    std::vector<std::unique_ptr<Choice>> choices;
};

} // namespace dnd

#endif // EFFECT_HOLDER_WITH_CHOICES_HPP_
