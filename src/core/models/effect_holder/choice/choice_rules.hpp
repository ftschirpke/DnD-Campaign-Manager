#ifndef CHOICE_RULES_HPP_
#define CHOICE_RULES_HPP_

#include <dnd_config.hpp>

#include <array>
#include <string>
#include <string_view>
#include <utility>

namespace dnd {

enum class ChoiceType {
    ABILITY,
    SKILL,
    STRING,
    EFFECT,
    ITEM,
    SPELL,
    CHOOSABLE_FEATURE,
};

/**
 * @brief Returns whether the given attribute name is a valid choice attribute name
 * @param attribute_name the attribute name to check
 * @return "true" if the given attribute name is a valid choice attribute name, "false" otherwise
 */
bool is_valid_choice_attribute_name(const std::string& attribute_name) noexcept;

/**
 * @brief Determines the choice type for the given attribute name
 * @param attribute_name the attribute name to determine the choice type for
 * @return the choice type for the given attribute name
 * @throws std::invalid_argument if the given attribute name is not a valid choice attribute name
 */
dnd::ChoiceType choice_type_for_attribute_name(const std::string& attribute_name);

/**
 * @brief Returns whether the given attribute name implies a group of strings
 * @param attribute_name the attribute name to check
 * @return "true" if the given attribute name implies a group of strings, "false" otherwise
 */
bool attribute_name_implies_group(const std::string& attribute_name) noexcept;

/**
 * @brief Determines the group name for the given attribute name if the attribute name implies a group of strings
 * @param attribute_name the attribute name to determine the group name for
 * @return the group name for the given attribute name if the attribute name implies a group of strings
 * @throws std::invalid_argument if the given attribute name is not a valid choice attribute name
 */
std::string group_name_for_attribute_name(const std::string& attribute_name);

} // namespace dnd

#endif // CHOICE_RULES_HPP_
