#ifndef IDENTIFIER_CONDITION_HPP_
#define IDENTIFIER_CONDITION_HPP_

#include <dnd_config.hpp>

#include <expected>
#include <string>
#include <string_view>

#include <core/errors/runtime_error.hpp>
#include <core/models/character/stats.hpp>
#include <core/models/effects/condition/condition.hpp>

namespace dnd {

class IdentifierCondition : public Condition {
public:
    IdentifierCondition(
        const std::string& left_side_identifier, ComparisonOperator comparison_operator,
        const std::string& right_side_identifier
    );
    IdentifierCondition(
        std::string_view left_side_identifier, ComparisonOperator comparison_operator,
        std::string_view right_side_identifier
    );

    std::expected<bool, RuntimeError> evaluate(const Stats& stats) const override final;
private:
    std::string right_side_identifier;
};

} // namespace dnd

#endif // IDENTIFIER_CONDITION_HPP_
