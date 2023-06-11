#include <dnd_config.hpp>

#include "condition_factory.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effect_holder/condition/condition.hpp>
#include <core/models/effect_holder/condition/identifier_condition.hpp>
#include <core/models/effect_holder/condition/literal_condition.hpp>
#include <core/validation/effect_holder/condition/condition_data.hpp>

std::unique_ptr<dnd::Condition> dnd::create_condition(dnd::ConditionData&& condition_data) {
    if (!condition_data.validate().ok()) {
        throw invalid_data("Cannot create condition from invalid data.");
    }
    std::string::const_iterator it = std::find(
        condition_data.condition_str.cbegin(), condition_data.condition_str.cend(), ' '
    );
    const std::string_view left_side_identifier = std::string_view(condition_data.condition_str.cbegin(), it);
    std::string::const_iterator last_it = ++it;
    it = std::find(it, condition_data.condition_str.cend(), ' ');
    const std::string_view operator_name = std::string_view(last_it, it);
    const std::string_view right_side_identifier = std::string_view(++it, condition_data.condition_str.cend());

    if (right_side_identifier[0] >= 'A' && right_side_identifier[0] <= 'Z') {
        return std::make_unique<IdentifierCondition>(left_side_identifier, operator_name, right_side_identifier);
    } else if (right_side_identifier == "true" || right_side_identifier == "false") {
        return std::make_unique<LiteralCondition>(left_side_identifier, operator_name, right_side_identifier == "true");
    } else if (right_side_identifier.find('.') != std::string::npos) {
        return std::make_unique<LiteralCondition>(
            left_side_identifier, operator_name, std::stof(right_side_identifier.data())
        );
    } else {
        return std::make_unique<LiteralCondition>(
            left_side_identifier, operator_name, std::stoi(right_side_identifier.data())
        );
    }
}
