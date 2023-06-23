#ifndef LITERAL_CONDITION_HPP_
#define LITERAL_CONDITION_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effect_holder/condition/condition.hpp>

namespace dnd {

/**
 * @brief A class that represents a condition that compares a character value to a literal (boolean, integer, or float)
 */
class LiteralCondition : public Condition {
public:
    /**
     * @brief Constructs a condition with the given left side identifier, operator, and right side boolean
     * CAREFUL: if the operator is not found, the construction doesn't fail, but the condition will always evaluate to
     * false
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     * @param right_side the right side of the condition
     */
    LiteralCondition(
        const std::string& left_side_identifier, const std::string& operator_name, bool right_side
    ) noexcept;
    /**
     * @brief Constructs a condition with the given left side identifier, operator, and right side boolean
     * CAREFUL: if the operator is not found, the construction doesn't fail, but the condition will always evaluate to
     * false
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     * @param right_side the right side of the condition
     */
    LiteralCondition(std::string_view left_side_identifier, std::string_view operator_name, bool right_side) noexcept;
    /**
     * @brief Constructs a condition with the given left side identifier, operator, and right side integer
     * CAREFUL: if the operator is not found, the construction doesn't fail, but the condition will always evaluate to
     * false
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     * @param right_side the right side of the condition
     */
    LiteralCondition(
        const std::string& left_side_identifier, const std::string& operator_name, int right_side
    ) noexcept;
    /**
     * @brief Constructs a condition with the given left side identifier, operator, and right side integer
     * CAREFUL: if the operator is not found, the construction doesn't fail, but the condition will always evaluate to
     * false
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     * @param right_side the right side of the condition
     */
    LiteralCondition(std::string_view left_side_identifier, std::string_view operator_name, int right_side) noexcept;
    /**
     * @brief Constructs a condition with the given left side identifier, operator, and right side float
     * CAREFUL: if the operator is not found, the construction doesn't fail, but the condition will always evaluate to
     * false
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     * @param right_side the right side of the condition
     */
    LiteralCondition(
        const std::string& left_side_identifier, const std::string& operator_name, float right_side
    ) noexcept;
    /**
     * @brief Constructs a condition with the given left side identifier, operator, and right side float
     * CAREFUL: if the operator is not found, the construction doesn't fail, but the condition will always evaluate to
     * false
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     * @param right_side the right side of the condition
     */
    LiteralCondition(std::string_view left_side_identifier, std::string_view operator_name, float right_side) noexcept;
    /**
     * @brief Evaluates the condition for given attributes and constants
     * @param attributes a map of attributes
     * @param constants a map of constants
     * @return "true" if the condition is fulfilled, "false" otherwise (or if the operator does not exist)
     * @throws std::out_of_range if the identifier used in the condition doesn't exist among the given attributes and
     * constants
     */
    virtual bool evaluate(
        const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override final;
private:
    int right_side;
};

} // namespace dnd

#endif // LITERAL_CONDITION_HPP_
