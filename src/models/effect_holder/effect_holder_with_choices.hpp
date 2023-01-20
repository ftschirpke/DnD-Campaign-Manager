#ifndef EFFECT_HOLDER_WITH_CHOICES_HPP_
#define EFFECT_HOLDER_WITH_CHOICES_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/extra_spells_holder.hpp"
#include "models/effect_holder/proficiency_holder.hpp"
#include "models/effect_holder/riv_holder.hpp"

namespace dnd {

class Choice {
public:
    int amount;
    std::string attribute_name;
};

template <typename T>
class OutOfChoice : public Choice {
public:
    std::vector<T> out_of;
};

class GroupChoice : public Choice {
public:
    std::string group_name;
};

class EffectHolderWithChoices : public EffectHolder {
public:
    // std::vector<std::unique_ptr<Choice>> choices;
    Choice choice;
};

} // namespace dnd

#endif // EFFECT_HOLDER_WITH_CHOICES_HPP_
