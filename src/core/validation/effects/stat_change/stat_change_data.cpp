#include <dnd_config.hpp>

#include "stat_change_data.hpp"

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

static constexpr const char*
    stat_change_regex_cstr = "[A-Z][_A-Z0-9]+ (earliest|early|normal|late|latest) ((add|sub|mult|div|set|max|min) "
                             "([A-Z][_A-Z0-9]+|-?[1-9]\\d*(\\.[1-9]|\\.\\d[1-9])?)|(set (false|true)))";

StatChangeData::StatChangeData(std::shared_ptr<ValidationData> parent) noexcept : ValidationSubdata(parent) {}

Errors validate_stat_change(const StatChangeData& data) {
    DND_MEASURE_FUNCTION();
    Errors errors;
    static const std::regex stat_change_regex(stat_change_regex_cstr);
    if (!std::regex_match(data.stat_change_str, stat_change_regex)) {
        std::string msg;
        if (data.stat_change_str.empty()) {
            msg = "Stat change cannot be empty";
        } else if (data.stat_change_str.ends_with("0")) {
            msg = fmt::format("Stat change \"{}\" ends with unnecessary 0", data.stat_change_str);
        } else {
            msg = fmt::format("Invalid stat change \"{}\"", data.stat_change_str);
        }
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(), std::move(msg));
    } else if (data.stat_change_str.ends_with("div 0")) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
            fmt::format("Division by zero in stat change \"{}\"", data.stat_change_str)
        );
    }
    return errors;
}

} // namespace dnd
