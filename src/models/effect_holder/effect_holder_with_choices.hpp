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
    const int amount;
    const std::string attribute_name;
    Choice(int amount, const std::string& attribute_name);
};

class SelectionChoice : public Choice {
public:
    std::vector<std::string> selection;
    SelectionChoice(int amount, const std::string& attribute_name, std::vector<std::string>&& selection);
};

class GroupChoice : public Choice {
public:
    std::vector<std::string> group_names;
    GroupChoice(int amount, const std::string& attribute_name, const std::string& group_name);
    GroupChoice(int amount, const std::string& attribute_name, std::vector<std::string>&& group_names);
};

class EffectHolderWithChoices : public EffectHolder {
public:
    std::vector<std::unique_ptr<Choice>> choices;
};

inline Choice::Choice(int amount, const std::string& attribute_name) : amount(amount), attribute_name(attribute_name) {}

inline SelectionChoice::SelectionChoice(
    int amount, const std::string& attribute_name, std::vector<std::string>&& selection
)
    : Choice(amount, attribute_name), selection(selection) {}

inline GroupChoice::GroupChoice(int amount, const std::string& attribute_name, const std::string& group_name)
    : Choice(amount, attribute_name), group_names({group_name}) {}

inline GroupChoice::GroupChoice(int amount, const std::string& attribute_name, std::vector<std::string>&& group_names)
    : Choice(amount, attribute_name), group_names(group_names) {}

} // namespace dnd

#endif // EFFECT_HOLDER_WITH_CHOICES_HPP_
