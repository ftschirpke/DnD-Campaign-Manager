#include <dnd_config.hpp>

#include "literal_condition.hpp"

#include <string>
#include <string_view>

#include <tl/expected.hpp>

#include <core/errors/runtime_error.hpp>
#include <core/models/character/stats.hpp>
#include <core/models/effects/condition/condition.hpp>

namespace dnd {

LiteralCondition::LiteralCondition(
    const std::string& left_side_identifier, ComparisonOperator comparison_operator, bool right_side
)
    : Condition(left_side_identifier, comparison_operator), right_side(right_side) {}

LiteralCondition::LiteralCondition(
    std::string_view left_side_identifier, ComparisonOperator comparison_operator, bool right_side
)
    : Condition(left_side_identifier, comparison_operator), right_side(right_side) {}

LiteralCondition::LiteralCondition(
    const std::string& left_side_identifier, ComparisonOperator comparison_operator, int right_side
)
    : Condition(left_side_identifier, comparison_operator), right_side(right_side * 100) {}

LiteralCondition::LiteralCondition(
    std::string_view left_side_identifier, ComparisonOperator comparison_operator, int right_side
)
    : Condition(left_side_identifier, comparison_operator), right_side(right_side * 100) {}

LiteralCondition::LiteralCondition(
    const std::string& left_side_identifier, ComparisonOperator comparison_operator, float right_side
)
    : Condition(left_side_identifier, comparison_operator), right_side(static_cast<int>(right_side * 100)) {}

LiteralCondition::LiteralCondition(
    std::string_view left_side_identifier, ComparisonOperator comparison_operator, float right_side
)
    : Condition(left_side_identifier, comparison_operator), right_side(static_cast<int>(right_side * 100)) {}

tl::expected<bool, RuntimeError> LiteralCondition::evaluate(const Stats& stats) const {
    return evaluate_with_right_side(stats, right_side);
}

} // namespace dnd
