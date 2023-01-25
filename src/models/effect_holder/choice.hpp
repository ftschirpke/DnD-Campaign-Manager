#ifndef CHOICE_HPP_
#define CHOICE_HPP_

#include "dnd_config.hpp"

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

class GroupChoice : public Choice {
public:
    std::vector<std::string> group_names;
    GroupChoice(int amount, const std::string& attribute_name, const std::string& group_name);
    GroupChoice(int amount, const std::string& attribute_name, std::vector<std::string>&& group_names);
    virtual bool isValidDecision(const std::string& decision_str) const;
};

inline Choice::Choice(int amount, const std::string& attribute_name) : amount(amount), attribute_name(attribute_name) {}

inline SelectionChoice::SelectionChoice(
    int amount, const std::string& attribute_name, std::vector<std::string>&& selection
)
    : Choice(amount, attribute_name), selection(std::move(selection)) {}

inline GroupChoice::GroupChoice(int amount, const std::string& attribute_name, const std::string& group_name)
    : Choice(amount, attribute_name), group_names({group_name}) {}

inline GroupChoice::GroupChoice(int amount, const std::string& attribute_name, std::vector<std::string>&& group_names)
    : Choice(amount, attribute_name), group_names(std::move(group_names)) {}

} // namespace dnd

#endif // CHOICE_HPP_
