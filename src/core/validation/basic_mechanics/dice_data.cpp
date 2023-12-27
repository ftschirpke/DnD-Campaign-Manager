#include <dnd_config.hpp>

#include "dice_data.hpp"

#include <regex>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

static constexpr const char* dice_regex_cstr = "(([1-9]\\d*)?[dD](4|6|8|10|12|20|100)\\+)*"
                                               "(([1-9]\\d*)?[dD](4|6|8|10|12|20|100))"
                                               "([\\+\\-]\\d+)?";

DiceData::DiceData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

Errors DiceData::validate() const {
    static const std::regex dice_regex(dice_regex_cstr);
    Errors errors;
    if (!std::regex_match(str, dice_regex)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, fmt::format("Invalid dice \"{}\"", str)
        );
    }
    return errors;
}

} // namespace dnd
