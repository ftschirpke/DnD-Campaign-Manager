#ifndef IDENTIFIER_CONDITION_HPP_
#define IDENTIFIER_CONDITION_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effects/condition/condition.hpp>

namespace dnd {

/**
 * @brief A class that represents a condition that compares two character values
 */
class IdentifierCondition : public Condition {
public:
    /**
     * @brief Constructs a condition with the given identifiers and operator
     * CAREFUL: if the operator is not found, the construction doesn't fail, but the condition will always evaluate to
     * false
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     * @param right_side_identifier the identifier on the right side of the condition
     */
    IdentifierCondition(
        const std::string& left_side_identifier, const std::string& operator_name,
        const std::string& right_side_identifier
    ) noexcept;
    /**
     * @brief Constructs a condition with the given identifiers and operator
     * CAREFUL: if the operator is not found, the construction doesn't fail, but the condition will always evaluate to
     * false
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     * @param right_side_identifier the identifier on the right side of the condition
     */
    IdentifierCondition(
        std::string_view left_side_identifier, std::string_view operator_name, std::string_view right_side_identifier
    ) noexcept;
    /**
     * @brief Evaluates the condition for given attributes and constants
     * @param attributes a map of attributes
     * @param constants a map of constants
     * @return "true" if the condition is fulfilled, "false" otherwise (or if the operator does not exist)
     * @throws std::out_of_range if the identifiers used in the condition don't exist among the given attributes and
     * constants
     */
    virtual bool evaluate(
        const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override final;
private:
    std::string right_side_identifier;
};

} // namespace dnd

#endif // IDENTIFIER_CONDITION_HPP_
