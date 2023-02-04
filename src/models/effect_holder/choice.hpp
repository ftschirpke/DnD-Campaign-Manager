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

/**
 * @brief A class representing a character having to make a choice between multiple options.
 */
class Choice {
public:
    /**
     * @brief Constructs choice with the amount to choose and the name of the attribute the choice affects
     * @param amount the amount of values that have to be chosen
     * @param attribute_name the name of the affected attribute
     */
    Choice(int amount, const std::string& attribute_name);
    virtual ~Choice() noexcept = default;
    /**
     * @brief Checks whether a given decision value is valid
     * @param decision_str the value of the decision
     * @return "true" if the chosen value is valid, "false" otherwise
     */
    virtual bool isValidDecision(const std::string& decision_str) const = 0;

    // the amount of values that have to be chosen
    const int amount;
    // the name of the attribute affected by the choice
    const std::string attribute_name;
};

/**
 * @brief A class representing a character having to choose between certain fixed values.
 */
class SelectionChoice : public Choice {
public:
    /**
     * @brief Constructs selection choice
     * @param amount the amount of values that have to be chosen
     * @param attribute_name the name of the affected attribute
     * @param selection the selection of values
     */
    SelectionChoice(int amount, const std::string& attribute_name, std::vector<std::string>&& selection);
    /**
     * @brief Checks whether a given decision value is valid i.e. is part of the possible values in the selection
     * @param decision_str the value of the decision
     * @return "true" if the chosen value is valid, "false" otherwise
     */
    virtual bool isValidDecision(const std::string& decision_str) const;

    // the selection of fixed values the character has to choose out of
    std::vector<std::string> selection;
};

/**
 * @brief A class representing a character having to choose values out of groups where each group contains choosables.
 */
class ChoosableGroupChoice : public Choice {
public:
    /**
     * @brief Constructs choosable group choice
     * @param amount the amount of choosables that have to be chosen
     * @param attribute_name the name of the affected attribute
     * @param group_values pointers to the groups the character can choose out of
     */
    ChoosableGroupChoice(
        int amount, const std::string& attribute_name,
        std::vector<const std::unordered_map<std::string, Choosable>*>&& group_values
    );
    /**
     * @brief Checks whether a given decision value is valid i.e. is part of the choosable group
     * @param decision_str the value of the decision
     * @return "true" if the chosen value is valid, "false" otherwise
     */
    virtual bool isValidDecision(const std::string& decision_str) const;

    // pointers to the groups the character can choose out of
    std::vector<const std::unordered_map<std::string, Choosable>*> group_values;
};


/**
 * @brief A class representing a character having to choose values out of groups where each group contains strings.
 */
class StringGroupChoice : public Choice {
public:
    /**
     * @brief Constructs string group choice
     * @param amount the amount of strings that have to be chosen
     * @param attribute_name the name of the affected attribute
     * @param group_values pointers to the groups the character can choose out of
     */
    StringGroupChoice(
        int amount, const std::string& attribute_name,
        std::vector<const std::unordered_set<std::string>*>&& group_values
    );
    /**
     * @brief Checks whether a given decision value is valid i.e. is part of the string group
     * @param decision_str the value of the decision
     * @return "true" if the chosen value is valid, "false" otherwise
     */
    virtual bool isValidDecision(const std::string& decision_str) const;

    // pointers to the groups the character can choose out of
    std::vector<const std::unordered_set<std::string>*> group_values;
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
