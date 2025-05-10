#include <dnd_config.hpp>

#include "condition_factory.hpp"

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include <core/data_result.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/condition/identifier_condition.hpp>
#include <core/models/effects/condition/literal_condition.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/effects/condition/condition_validation.hpp>

namespace dnd {

static std::optional<ComparisonOperator> comparison_operator_from_str(std::string_view str) {
    if (str == "==") {
        return ComparisonOperator::EQUAL;
    } else if (str == "!=") {
        return ComparisonOperator::NOT_EQUAL;
    } else if (str == "<") {
        return ComparisonOperator::LESS_THAN;
    } else if (str == "<=") {
        return ComparisonOperator::LESS_THAN_OR_EQUAL;
    } else if (str == ">") {
        return ComparisonOperator::GREATER_THAN;
    } else if (str == ">=") {
        return ComparisonOperator::GREATER_THAN_OR_EQUAL;
    }
    return std::nullopt;
}

FactoryResult<Condition> create_condition(Condition::Data&& data) {
    Errors errors = validate_condition(data);
    if (!errors.ok()) {
        return InvalidFactory<Condition>(std::move(data), std::move(errors));
    }

    std::string::const_iterator it = std::find(data.condition_str.cbegin(), data.condition_str.cend(), ' ');
    const std::string_view left_side_identifier = str_view(data.condition_str.cbegin(), it);
    std::string::const_iterator last_it = ++it;
    it = std::find(it, data.condition_str.cend(), ' ');
    const std::string_view operator_name = str_view(last_it, it);
    const std::string_view right_side_identifier = str_view(++it, data.condition_str.cend());

    std::optional<ComparisonOperator> comparison_operator_optional = comparison_operator_from_str(operator_name);
    if (!comparison_operator_optional.has_value()) {
        Errors sub_errors(ValidationError(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Invalid comparison operator '{}'", operator_name)
        ));
        return InvalidFactory<Condition>(std::move(data), std::move(sub_errors));
    }
    ComparisonOperator comparison_operator = comparison_operator_optional.value();

    std::unique_ptr<Condition> condition;
    if (right_side_identifier[0] >= 'A' && right_side_identifier[0] <= 'Z') {
        condition = std::make_unique<IdentifierCondition>(
            left_side_identifier, comparison_operator, right_side_identifier
        );
    } else if (right_side_identifier == "true" || right_side_identifier == "false") {
        condition = std::make_unique<LiteralCondition>(
            left_side_identifier, comparison_operator, right_side_identifier == "true"
        );
    } else if (right_side_identifier.find('.') != std::string::npos) {
        condition = std::make_unique<LiteralCondition>(
            left_side_identifier, comparison_operator, std::stof(std::string(right_side_identifier))
        );
    } else {
        condition = std::make_unique<LiteralCondition>(
            left_side_identifier, comparison_operator, std::stoi(std::string(right_side_identifier))
        );
    }
    return ValidFactory(std::move(condition));
}

} // namespace dnd
