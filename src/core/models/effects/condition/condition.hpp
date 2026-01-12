#ifndef CONDITION_HPP_
#define CONDITION_HPP_

#include <dnd_config.hpp>

#include <expected>
#include <string>
#include <string_view>

#include <core/errors/runtime_error.hpp>
#include <core/models/character/stats.hpp>

namespace dnd {

enum class ComparisonOperator {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
};

class Condition {
public:
    struct Data;

    virtual ~Condition() = default;
    virtual std::expected<bool, RuntimeError> evaluate(const Stats& stats) const = 0;
protected:
    Condition(const std::string& left_side_identifier, ComparisonOperator comparison_operator);
    Condition(std::string_view left_side_identifier, ComparisonOperator comparison_operator);

    std::expected<bool, RuntimeError> evaluate_with_right_side(const Stats& stats, int right_side_value) const;

    std::string left_side_identifier;
    ComparisonOperator comparison_operator;
};

struct Condition::Data {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::string condition_str;
};

} // namespace dnd

#endif // CONDITION_HPP_
