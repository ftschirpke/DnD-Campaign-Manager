#ifndef CONDITION_HPP_
#define CONDITION_HPP_

#include <dnd_config.hpp>

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace dnd {

/**
 * @brief A class that represents a condition that a character or create may fulfill
 */
class Condition {
public:
    /**
     * @brief Constructs a condition with the given left side identifier and operator
     * CAREFUL: if the operator is not found, the construction doesn't fail
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     */
    Condition(const std::string& left_side_identifier, const std::string& operator_name) noexcept;
    /**
     * @brief Constructs a condition with the given left side identifier and operator
     * CAREFUL: if the operator is not found, the construction doesn't fail
     * @param left_side_identifier the identifier on the left side of the condition
     * @param operator_name the name of the operator
     */
    Condition(std::string_view left_side_identifier, std::string_view operator_name) noexcept;
    virtual ~Condition() noexcept = default;
    /**
     * @brief Evaluates the condition for given attributes and constants
     * @param attributes a map of attributes
     * @param constants a map of constants
     * @return "true" if the condition is fulfilled, "false" otherwise
     * @throws std::out_of_range if the identifier used in the condition doesn't exist among the given attributes and
     * constants
     */
    virtual bool evaluate(
        const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const = 0;
protected:
    std::string left_side_identifier;
    std::function<bool(int, int)> comparison_operator;
};

} // namespace dnd

#endif // CONDITION_HPP_
