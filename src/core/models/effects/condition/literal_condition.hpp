#ifndef LITERAL_CONDITION_HPP_
#define LITERAL_CONDITION_HPP_

#include <dnd_config.hpp>

#include <expected>
#include <string>
#include <string_view>

#include <core/errors/runtime_error.hpp>
#include <core/models/character/stats.hpp>
#include <core/models/effects/condition/condition.hpp>

namespace dnd {

/**
 * @brief A class that represents a condition that compares a character value to a literal (boolean, integer, or float)
 */
class LiteralCondition : public Condition {
public:
    LiteralCondition(const std::string& left_side_identifier, ComparisonOperator comparison_operator, bool right_side);
    LiteralCondition(std::string_view left_side_identifier, ComparisonOperator comparison_operator, bool right_side);
    LiteralCondition(const std::string& left_side_identifier, ComparisonOperator comparison_operator, int right_side);
    LiteralCondition(std::string_view left_side_identifier, ComparisonOperator comparison_operator, int right_side);
    LiteralCondition(const std::string& left_side_identifier, ComparisonOperator comparison_operator, float right_side);
    LiteralCondition(std::string_view left_side_identifier, ComparisonOperator comparison_operator, float right_side);

    std::expected<bool, RuntimeError> evaluate(const Stats& stats) const override final;
private:
    int right_side;
};

} // namespace dnd

#endif // LITERAL_CONDITION_HPP_
