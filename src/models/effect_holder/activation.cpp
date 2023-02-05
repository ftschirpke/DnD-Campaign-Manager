#include "dnd_config.hpp"

#include "activation.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>

constexpr std::array<std::pair<const char*, bool (*)(int, int)>, 6> dnd::Activation::operators = {
    std::make_pair("==", [](int a, int b) { return a == b; }),
    std::make_pair("!=", [](int a, int b) { return a != b; }),
    std::make_pair(">=", [](int a, int b) { return a >= b; }),
    std::make_pair("<=", [](int a, int b) { return a <= b; }),
    std::make_pair(">", [](int a, int b) { return a > b; }),
    std::make_pair("<", [](int a, int b) { return a < b; }),
};

bool dnd::NumericActivation::check(
    const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    int left_value;
    try {
        if (constants.contains(left_identifier)) {
            left_value = constants.at(left_identifier);
        } else {
            left_value = attributes.at(left_identifier);
        }
    } catch (const std::out_of_range& e) {
        DND_UNUSED(e);
        throw std::out_of_range("\"" + left_identifier + "\" does not exist. It is neither an attribute nor constant.");
    }
    return op(left_value, right_value);
}

bool dnd::IdentifierActivation::check(
    const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    int left_value, right_value;
    try {
        if (constants.contains(left_identifier)) {
            left_value = constants.at(left_identifier);
        } else {
            left_value = attributes.at(left_identifier);
        }
    } catch (const std::out_of_range& e) {
        DND_UNUSED(e);
        throw std::out_of_range("\"" + left_identifier + "\" does not exist. It is neither an attribute or constant.");
    }
    try {
        if (constants.contains(right_identifier)) {
            right_value = constants.at(right_identifier);
        } else {
            right_value = attributes.at(right_identifier);
        }
    } catch (const std::out_of_range& e) {
        DND_UNUSED(e);
        throw std::out_of_range("\"" + right_identifier + "\" does not exist. It is neither an attribute or constant.");
    }
    return op(left_value, right_value);
}
