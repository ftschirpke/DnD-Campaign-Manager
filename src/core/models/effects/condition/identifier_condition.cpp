#include <dnd_config.hpp>

#include "identifier_condition.hpp"

#include <optional>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <tl/expected.hpp>

#include <core/errors/runtime_error.hpp>
#include <core/models/character/stats.hpp>
#include <core/models/effects/condition/condition.hpp>

namespace dnd {

IdentifierCondition::IdentifierCondition(
    const std::string& left_side_identifier, ComparisonOperator comparison_operator,
    const std::string& right_side_identifier
)
    : Condition(left_side_identifier, comparison_operator), right_side_identifier(right_side_identifier) {}

IdentifierCondition::IdentifierCondition(
    std::string_view left_side_identifier, ComparisonOperator comparison_operator,
    std::string_view right_side_identifier
)
    : Condition(left_side_identifier, comparison_operator), right_side_identifier(right_side_identifier) {}

tl::expected<bool, RuntimeError> IdentifierCondition::evaluate(const Stats& stats) const {
    std::optional<int> right_side_optional = stats.get(right_side_identifier);
    if (!right_side_optional.has_value()) {
        return tl::unexpected(RuntimeError(
            RuntimeError::Code::INVALID_ARGUMENT,
            fmt::format("Condition right side identifier '{}' not found in stats", right_side_identifier)
        ));
    }
    int right_side_value = right_side_optional.value();
    return evaluate_with_right_side(stats, right_side_value);
}

} // namespace dnd
