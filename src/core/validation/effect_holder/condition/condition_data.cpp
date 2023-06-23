#include <dnd_config.hpp>

#include "condition_data.hpp"

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

static constexpr const char*
    condition_regex_cstr = "[A-Z][_A-Z0-9]+ (==|!=|>=|<=|>|<) ([A-Z][_A-Z0-9]+|-?\\d+(\\.\\d\\d?)?|true|false)";

dnd::ConditionData::ConditionData(const dnd::ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::ConditionData::validate() const {
    DND_MEASURE_FUNCTION();
    static const std::regex condition_regex(condition_regex_cstr);
    Errors errors;
    if (!std::regex_match(condition_str, condition_regex)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, fmt::format("Invalid condition \"{}\"", condition_str)
        );
    }
    return errors;
}
