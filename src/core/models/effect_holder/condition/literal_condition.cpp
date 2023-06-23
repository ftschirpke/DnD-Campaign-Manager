#include <dnd_config.hpp>

#include "literal_condition.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effect_holder/condition/condition.hpp>

dnd::LiteralCondition::LiteralCondition(
    const std::string& left_side_identifier, const std::string& operator_name, bool right_side
) noexcept
    : Condition(left_side_identifier, operator_name), right_side(right_side) {}

dnd::LiteralCondition::LiteralCondition(
    std::string_view left_side_identifier, std::string_view operator_name, bool right_side
) noexcept
    : Condition(left_side_identifier, operator_name), right_side(right_side) {}

dnd::LiteralCondition::LiteralCondition(
    const std::string& left_side_identifier, const std::string& operator_name, int right_side
) noexcept
    : Condition(left_side_identifier, operator_name), right_side(right_side * 100) {}

dnd::LiteralCondition::LiteralCondition(
    std::string_view left_side_identifier, std::string_view operator_name, int right_side
) noexcept
    : Condition(left_side_identifier, operator_name), right_side(right_side * 100) {}

dnd::LiteralCondition::LiteralCondition(
    const std::string& left_side_identifier, const std::string& operator_name, float right_side
) noexcept
    : Condition(left_side_identifier, operator_name), right_side(static_cast<int>(right_side * 100)) {}

dnd::LiteralCondition::LiteralCondition(
    std::string_view left_side_identifier, std::string_view operator_name, float right_side
) noexcept
    : Condition(left_side_identifier, operator_name), right_side(static_cast<int>(right_side * 100)) {}

bool dnd::LiteralCondition::evaluate(
    const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    if (comparison_operator == nullptr) {
        return false;
    } else if (attributes.contains(left_side_identifier)) {
        return comparison_operator(attributes.at(left_side_identifier), right_side);
    } else if (constants.contains(left_side_identifier)) {
        return comparison_operator(constants.at(left_side_identifier), right_side);
    }
    throw std::out_of_range("Identifier \"" + left_side_identifier + "\" not found.");
}
