#include <dnd_config.hpp>

#include "condition.hpp"

#include <cassert>
#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include <core/errors/runtime_error.hpp>
#include <core/models/character/stats.hpp>

namespace dnd {

Condition::Condition(const std::string& left_side_identifier, ComparisonOperator comparison_operator)
    : left_side_identifier(left_side_identifier), comparison_operator(comparison_operator) {}

Condition::Condition(std::string_view left_side_identifier, ComparisonOperator comparison_operator)
    : left_side_identifier(left_side_identifier), comparison_operator(comparison_operator) {}

std::expected<bool, RuntimeError> Condition::evaluate_with_right_side(const Stats& stats, int right_side_value) const {
    std::optional<int> left_side_optional = stats.get_raw(left_side_identifier);
    if (!left_side_optional.has_value()) {
        return std::unexpected(RuntimeError(
            RuntimeError::Code::INVALID_ARGUMENT,
            fmt::format("Condition left side identifier '{}' not found in stats", left_side_identifier)
        ));
    }
    int left_side_value = left_side_optional.value();
    switch (comparison_operator) {
        case ComparisonOperator::EQUAL:
            return left_side_value == right_side_value;
        case ComparisonOperator::NOT_EQUAL:
            return left_side_value != right_side_value;
        case ComparisonOperator::LESS_THAN:
            return left_side_value < right_side_value;
        case ComparisonOperator::LESS_THAN_OR_EQUAL:
            return left_side_value <= right_side_value;
        case ComparisonOperator::GREATER_THAN:
            return left_side_value > right_side_value;
        case ComparisonOperator::GREATER_THAN_OR_EQUAL:
            return left_side_value >= right_side_value;
    }
    std::unreachable();
}

} // namespace dnd
