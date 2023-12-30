#include <dnd_config.hpp>

#include "condition_factory.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/condition/identifier_condition.hpp>
#include <core/models/effects/condition/literal_condition.hpp>
#include <core/utils/data_result.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/effects/condition/condition_data.hpp>

namespace dnd {

FactoryResult<Condition> create_condition(Condition::Data&& data) {
    Errors errors = data.validate();
    if (!errors.ok()) {
        return InvalidFactory<Condition>(std::move(data), std::move(errors));
    }

    std::string::const_iterator it = std::find(data.condition_str.cbegin(), data.condition_str.cend(), ' ');
    const std::string_view left_side_identifier = str_view(data.condition_str.cbegin(), it);
    std::string::const_iterator last_it = ++it;
    it = std::find(it, data.condition_str.cend(), ' ');
    const std::string_view operator_name = str_view(last_it, it);
    const std::string_view right_side_identifier = str_view(++it, data.condition_str.cend());

    std::unique_ptr<Condition> condition;
    if (right_side_identifier[0] >= 'A' && right_side_identifier[0] <= 'Z') {
        condition = std::make_unique<IdentifierCondition>(left_side_identifier, operator_name, right_side_identifier);
    } else if (right_side_identifier == "true" || right_side_identifier == "false") {
        condition = std::make_unique<LiteralCondition>(
            left_side_identifier, operator_name, right_side_identifier == "true"
        );
    } else if (right_side_identifier.find('.') != std::string::npos) {
        condition = std::make_unique<LiteralCondition>(
            left_side_identifier, operator_name, std::stof(right_side_identifier.data())
        );
    } else {
        condition = std::make_unique<LiteralCondition>(
            left_side_identifier, operator_name, std::stoi(right_side_identifier.data())
        );
    }
    return ValidFactory(std::move(condition));
}

} // namespace dnd
