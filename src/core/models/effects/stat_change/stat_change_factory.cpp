#include <dnd_config.hpp>

#include "stat_change_factory.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/effects/stat_change/identifier_stat_change.hpp>
#include <core/models/effects/stat_change/literal_stat_change.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/effects/stat_change/stat_change_validation.hpp>

namespace dnd {

static std::optional<StatChange::Time> stat_change_time_from_str(std::string_view stat_change_time_str) {
    if (stat_change_time_str == "earliest") {
        return StatChange::Time::EARLIEST;
    } else if (stat_change_time_str == "early") {
        return StatChange::Time::EARLY;
    } else if (stat_change_time_str == "normal") {
        return StatChange::Time::NORMAL;
    } else if (stat_change_time_str == "late") {
        return StatChange::Time::LATE;
    } else if (stat_change_time_str == "latest") {
        return StatChange::Time::LATEST;
    }
    std::unreachable();
}

static std::optional<StatChange::Operation> stat_change_operation_from_str(std::string_view stat_change_operation_str) {
    if (stat_change_operation_str == "add") {
        return StatChange::Operation::ADD;
    } else if (stat_change_operation_str == "sub") {
        return StatChange::Operation::SUB;
    } else if (stat_change_operation_str == "mult") {
        return StatChange::Operation::MULT;
    } else if (stat_change_operation_str == "div") {
        return StatChange::Operation::DIV;
    } else if (stat_change_operation_str == "set") {
        return StatChange::Operation::SET;
    } else if (stat_change_operation_str == "max") {
        return StatChange::Operation::MAX;
    } else if (stat_change_operation_str == "min") {
        return StatChange::Operation::MIN;
    }
    std::unreachable();
}

FactoryResult<StatChange> create_stat_change(StatChange::Data&& data) {
    Errors errors = validate_stat_change(data);
    if (!errors.ok()) {
        return InvalidFactory<StatChange>(std::move(data), std::move(errors));
    }
    std::string::const_iterator it = std::find(data.stat_change_str.cbegin(), data.stat_change_str.cend(), ' ');
    const std::string_view affected_attribute = str_view(data.stat_change_str.cbegin(), it);
    std::string::const_iterator last_it = ++it;
    it = std::find(it, data.stat_change_str.cend(), ' ');

    const std::string_view stat_change_time_str = str_view(last_it, it);
    std::optional<StatChange::Time> stat_change_time_optional = stat_change_time_from_str(stat_change_time_str);
    if (!stat_change_time_optional.has_value()) {
        Errors sub_errors = Errors(ValidationError(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Invalid stat change time '{}'", stat_change_time_str)
        ));
        return InvalidFactory<StatChange>(std::move(data), std::move(sub_errors));
    }
    StatChange::Time stat_change_time = stat_change_time_optional.value();

    last_it = ++it;
    it = std::find(it, data.stat_change_str.cend(), ' ');
    const std::string_view operator_name = str_view(last_it, it);

    std::optional<StatChange::Operation> operation_optional = stat_change_operation_from_str(operator_name);
    if (!operation_optional.has_value()) {
        Errors sub_errors = Errors(ValidationError(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Invalid stat change operation '{}'", operator_name)
        ));
        return InvalidFactory<StatChange>(std::move(data), std::move(sub_errors));
    }
    StatChange::Operation operation = operation_optional.value();

    const std::string_view value = str_view(++it, data.stat_change_str.cend());

    std::unique_ptr<StatChange> stat_change;
    if (value[0] >= 'A' && value[0] <= 'Z') {
        stat_change = std::make_unique<IdentifierStatChange>(affected_attribute, stat_change_time, operation, value);
    } else if (value == "true" || value == "false") {
        if (operation != StatChange::Operation::SET) {
            Errors sub_errors = Errors(ValidationError(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Boolean values only support the 'set' operation, '{}' is invalid.", operator_name)
            ));
            return InvalidFactory<StatChange>(std::move(data), std::move(sub_errors));
        }
        stat_change = std::make_unique<LiteralStatChange>(
            affected_attribute, stat_change_time, operation, value == "true"
        );
    } else if (value.find('.') != std::string::npos) {
        stat_change = std::make_unique<LiteralStatChange>(
            affected_attribute, stat_change_time, operation, std::stof(std::string(value))
        );
    } else {
        stat_change = std::make_unique<LiteralStatChange>(
            affected_attribute, stat_change_time, operation, std::stoi(std::string(value))
        );
    }
    return ValidFactory(std::move(stat_change));
}

} // namespace dnd
