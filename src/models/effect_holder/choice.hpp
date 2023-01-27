#ifndef CHOICE_HPP_
#define CHOICE_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "models/effect_holder/choosable.hpp"

namespace dnd {

class Choice {
public:
    const int amount;
    const std::string attribute_name;
    Choice(int amount, const std::string& attribute_name);
    virtual bool isValidDecision(const std::string& decision_str) const = 0;
};

class SelectionChoice : public Choice {
public:
    std::vector<std::string> selection;
    SelectionChoice(int amount, const std::string& attribute_name, std::vector<std::string>&& selection);
    virtual bool isValidDecision(const std::string& decision_str) const;
};

class ChoosableGroupChoice : public Choice {
public:
    std::vector<const std::unordered_map<std::string, Choosable>*> group_values;
    ChoosableGroupChoice(
        int amount, const std::string& attribute_name,
        std::vector<const std::unordered_map<std::string, Choosable>*>&& group_values
    );
    virtual bool isValidDecision(const std::string& decision_str) const;
};

class StringGroupChoice : public Choice {
public:
    std::vector<const std::unordered_set<std::string>*> group_values;
    StringGroupChoice(
        int amount, const std::string& attribute_name,
        std::vector<const std::unordered_set<std::string>*>&& group_values
    );
    virtual bool isValidDecision(const std::string& decision_str) const;
};

inline Choice::Choice(int amount, const std::string& attribute_name) : amount(amount), attribute_name(attribute_name) {}

inline SelectionChoice::SelectionChoice(
    int amount, const std::string& attribute_name, std::vector<std::string>&& selection
)
    : Choice(amount, attribute_name), selection(std::move(selection)) {}

inline ChoosableGroupChoice::ChoosableGroupChoice(
    int amount, const std::string& attribute_name,
    std::vector<const std::unordered_map<std::string, Choosable>*>&& group_values
)
    : Choice(amount, attribute_name), group_values(std::move(group_values)) {}

inline StringGroupChoice::StringGroupChoice(
    int amount, const std::string& attribute_name, std::vector<const std::unordered_set<std::string>*>&& group_values
)
    : Choice(amount, attribute_name), group_values(std::move(group_values)) {}

} // namespace dnd

#endif // CHOICE_HPP_
