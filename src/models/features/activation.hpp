#ifndef ACTIVATION_HPP_
#define ACTIVATION_HPP_

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace dnd {

const std::unordered_map<std::string, bool (*)(int, int)> activation_operators = {
    {"==", [](int a, int b) { return a == b; }}, {"!=", [](int a, int b) { return a != b; }},
    {">=", [](int a, int b) { return a >= b; }}, {"<=", [](int a, int b) { return a <= b; }},
    {">", [](int a, int b) { return a > b; }},   {"<", [](int a, int b) { return a < b; }},
};

class Activation {
public:
    const std::string left_identifier, op_name;
    std::unordered_map<std::string, bool (*)(int, int)>::mapped_type op;
    Activation(const std::string& left_identifier, const std::string& op_name);
    virtual bool isActive(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const = 0;
};

class NumericActivation : public Activation {
public:
    const int right_value;
    NumericActivation(const std::string& left_identifier, const std::string& op_name, int right_value);
    virtual bool isActive(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
};

class IdentifierActivation : public Activation {
public:
    const std::string right_identifier;
    IdentifierActivation(
        const std::string& left_identifier, const std::string& op_name, const std::string& right_identifier
    );
    virtual bool isActive(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
};


inline Activation::Activation(const std::string& left_identifier, const std::string& op_name)
    : left_identifier(left_identifier), op_name(op_name), op(nullptr) {
    try {
        op = activation_operators.at(op_name);
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Operator \"" + op_name + "\" does not exist.");
    }
}

inline NumericActivation::NumericActivation(
    const std::string& left_identifier, const std::string& op_name, int right_value
)
    : Activation(left_identifier, op_name), right_value(right_value) {}

inline bool NumericActivation::isActive(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    int left_value;
    try {
        if (constants.find(left_identifier) != constants.cend()) {
            left_value = constants.at(left_identifier);
        } else {
            left_value = attributes.at(left_identifier);
        }
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("\"" + left_identifier + "\" does not exist. It is neither an attribute or constant.");
    }
    return op(left_value, right_value);
}

inline IdentifierActivation::IdentifierActivation(
    const std::string& left_identifier, const std::string& op_name, const std::string& right_identifier
)
    : Activation(left_identifier, op_name), right_identifier(right_identifier) {}

inline bool IdentifierActivation::isActive(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    int left_value, right_value;
    try {
        if (constants.find(left_identifier) != constants.cend()) {
            left_value = constants.at(left_identifier);
        } else {
            left_value = attributes.at(left_identifier);
        }
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("\"" + left_identifier + "\" does not exist. It is neither an attribute or constant.");
    }
    try {
        if (constants.find(right_identifier) != constants.cend()) {
            right_value = constants.at(right_identifier);
        } else {
            right_value = attributes.at(right_identifier);
        }
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("\"" + right_identifier + "\" does not exist. It is neither an attribute or constant.");
    }
    return op(left_value, right_value);
}

} // namespace dnd

#endif // ACTIVATION_HPP_
