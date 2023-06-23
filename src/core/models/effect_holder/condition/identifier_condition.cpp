#include <dnd_config.hpp>

#include "identifier_condition.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effect_holder/condition/condition.hpp>

dnd::IdentifierCondition::IdentifierCondition(
    const std::string& left_side_identifier, const std::string& operator_name, const std::string& right_side_identifier
) noexcept
    : Condition(left_side_identifier, operator_name), right_side_identifier(right_side_identifier) {}

dnd::IdentifierCondition::IdentifierCondition(
    std::string_view left_side_identifier, std::string_view operator_name, std::string_view right_side_identifier
) noexcept
    : Condition(left_side_identifier, operator_name), right_side_identifier(right_side_identifier) {}

bool dnd::IdentifierCondition::evaluate(
    const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    if (comparison_operator == nullptr) {
        return false;
    }
    int left_side_value;
    if (attributes.contains(left_side_identifier)) {
        left_side_value = attributes.at(left_side_identifier);
    } else if (constants.contains(left_side_identifier)) {
        left_side_value = constants.at(left_side_identifier);
    } else {
        throw std::out_of_range("Identifier \"" + left_side_identifier + "\" not found.");
    }
    int right_side_value;
    if (attributes.contains(right_side_identifier)) {
        right_side_value = attributes.at(right_side_identifier);
    } else if (constants.contains(right_side_identifier)) {
        right_side_value = constants.at(right_side_identifier);
    } else {
        throw std::out_of_range("Identifier \"" + right_side_identifier + "\" not found.");
    }
    return comparison_operator(left_side_value, right_side_value);
}
