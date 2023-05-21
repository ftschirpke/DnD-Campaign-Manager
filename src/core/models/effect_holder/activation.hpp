#ifndef ACTIVATION_HPP_
#define ACTIVATION_HPP_

#include "dnd_config.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace dnd {

/**
 * @brief A class that represents an activation condition e.g. for a feature
 */
class Activation {
public:
    /**
     * @brief Constructs the activation with left part of the comparison and its operator function
     * @param left_identifier left part of the comparison
     * @param op_name string representation of the comparison operator
     * @throws std::out_of_range if op_name is not a valid name for an operator function
     */
    Activation(const std::string& left_identifier, const std::string& op_name);
    Activation(Activation&& other) noexcept = default;
    virtual ~Activation() noexcept = default;
    /**
     * @brief Checks whether the activation is fulfilled for given character attributes and character constants
     * @param attributes a map of character attributes
     * @param constants a map of character constants
     * @return "true" if attributes and constants fulfill activation condition, "false" otherwise
     * @throws std::out_of_range if left_identifier of the activation doesn't exist among the attributes and constants
     */
    virtual bool check(
        const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const = 0;

    // the supported comparison operators for activations
    static const std::array<std::pair<const char*, bool (*)(int, int)>, 6> operators;
    // left part of the comparison which must be an attribute or constant of the character
    const std::string left_identifier;
    // the operator in its string representation
    const std::string op_name;
    // the activation's comparison operator function
    std::unordered_map<std::string, bool (*)(int, int)>::mapped_type op;
};

/**
 * @brief A class that represents a numeric activation condition
 * i.e. an attribute or constant of the character needs to meet a certain numeric value
 */
class NumericActivation : public Activation {
public:
    // numeric value on the right side of the comparison
    const int right_value;
    /**
     * @brief Constructs the numeric activation
     * @param left_identifier left part of the comparison
     * @param op_name string representation of the comparison operator
     * @param right_value integer value of the numeric value on the right side of the comparison
     * @throws std::invalid_argument if op_name is not a valid name for an operator function
     */
    NumericActivation(const std::string& left_identifier, const std::string& op_name, int right_value);
    /**
     * @brief Checks whether the numeric activation is fulfilled for given character attributes and character constants
     * @param attributes a map of character attributes
     * @param constants a map of character constants
     * @return "true" if attributes and constants fulfill activation condition, "false" otherwise
     * @throws std::out_of_range if left_identifier of the activation doesn't exist among the attributes and constants
     */
    virtual bool check(
        const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
};

/**
 * @brief A class that represents an identifier activation condition
 * i.e. two identifiers (attributes or constants) of the character need to be in a certain relation to eachother
 */
class IdentifierActivation : public Activation {
public:
    // right part of the comparison which must be an attribute or constant of the character
    const std::string right_identifier;
    /**
     * @brief Constructs the identifier activation
     * @param left_identifier left part of the comparison (attribute or constant)
     * @param op_name string representation of the comparison operator
     * @param right_identifier right part of the comparison
     * @throws std::invalid_argument if op_name is not a valid name for an operator function
     */
    IdentifierActivation(
        const std::string& left_identifier, const std::string& op_name, const std::string& right_identifier
    );
    /**
     * @brief Checks whether the identifier activation is fulfilled for given character attributes and character
     * constants
     * @param attributes a map of character attributes
     * @param constants a map of character constants
     * @return "true" if attributes and constants fulfill activation condition, "false" otherwise
     * @throws std::out_of_range if left_identifier of the activation doesn't exist among the attributes and constants
     * @throws std::out_of_range if right_identifier of the activation doesn't exist among the attributes and constants
     */
    virtual bool check(
        const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
};

inline Activation::Activation(const std::string& left_identifier, const std::string& op_name)
    : left_identifier(left_identifier), op_name(op_name), op(nullptr) {
    for (const auto& [operator_name, operator_func] : operators) {
        if (operator_name == op_name) {
            op = operator_func;
            return;
        }
    }
    throw std::out_of_range("Operator \"" + op_name + "\" does not exist.");
}

inline NumericActivation::NumericActivation(
    const std::string& left_identifier, const std::string& op_name, int right_value
)
    : Activation(left_identifier, op_name), right_value(right_value) {}

inline IdentifierActivation::IdentifierActivation(
    const std::string& left_identifier, const std::string& op_name, const std::string& right_identifier
)
    : Activation(left_identifier, op_name), right_identifier(right_identifier) {}

} // namespace dnd

#endif // ACTIVATION_HPP_