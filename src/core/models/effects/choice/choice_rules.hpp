#ifndef CHOICE_RULES_HPP_
#define CHOICE_RULES_HPP_

#include <dnd_config.hpp>

#include <optional>
#include <string>

namespace dnd {

enum class ChoiceType {
    ABILITY,
    SKILL,
    STRING,
    STAT_CHANGE,
    ITEM,
    SPELL,
    CHOOSABLE,
};

bool is_valid_choice_attribute_name(const std::string& attribute_name);
std::optional<ChoiceType> choice_type_for_attribute_name(const std::string& attribute_name);

bool attribute_name_implies_group(const std::string& attribute_name);
std::optional<std::string> group_name_for_attribute_name(const std::string& attribute_name);

} // namespace dnd

#endif // CHOICE_RULES_HPP_
