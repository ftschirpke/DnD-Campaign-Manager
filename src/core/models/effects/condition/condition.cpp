#include <dnd_config.hpp>

#include "condition.hpp"

#include <array>
#include <string>
#include <string_view>
#include <utility>

namespace dnd {

static constexpr std::array<std::pair<const char*, bool (*)(int, int)>, 6> comparison_operators = {
    std::pair("==", [](int a, int b) { return a == b; }), std::pair("!=", [](int a, int b) { return a != b; }),
    std::pair(">=", [](int a, int b) { return a >= b; }), std::pair("<=", [](int a, int b) { return a <= b; }),
    std::pair(">", [](int a, int b) { return a > b; }),   std::pair("<", [](int a, int b) { return a < b; }),
};

Condition::Condition(const std::string& left_side_identifier, const std::string& operator_name)
    : left_side_identifier(left_side_identifier), comparison_operator(nullptr) {
    for (const auto& [op_name, op_func] : comparison_operators) {
        if (op_name == operator_name) {
            comparison_operator = op_func;
            break;
        }
    }
}

Condition::Condition(std::string_view left_side_identifier, std::string_view operator_name)
    : left_side_identifier(left_side_identifier), comparison_operator(nullptr) {
    for (const auto& [op_name, op_func] : comparison_operators) {
        if (op_name == operator_name) {
            comparison_operator = op_func;
            break;
        }
    }
}

} // namespace dnd
