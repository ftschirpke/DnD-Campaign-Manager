#include <dnd_config.hpp>

#include "stat_change_data.hpp"

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

static constexpr const char*
    stat_change_regex_cstr = "[A-Z][_A-Z0-9]+ (earliest|early|normal|late|latest) ((add|sub|mult|div|set|max|min) "
                             "([A-Z][_A-Z0-9]+|-?\\d+(\\.[1-9]|\\.\\d[1-9])?)|(set (false|true)))";

dnd::StatChangeData::StatChangeData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::StatChangeData::validate() const {
    DND_MEASURE_FUNCTION();
    Errors errors;
    static const std::regex stat_change_regex(stat_change_regex_cstr);
    if (!std::regex_match(stat_change_str, stat_change_regex)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format("Invalid stat change \"{}\"", stat_change_str)
        );
    } else if (stat_change_str.ends_with("div 0")) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format("Division by zero in stat change \"{}\"", stat_change_str)
        );
    }
    return errors;
}
