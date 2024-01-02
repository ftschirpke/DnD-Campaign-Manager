#include <dnd_config.hpp>

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>

namespace dnd {

static constexpr const char* dice_regex_cstr = "(([1-9]\\d*)?[dD](4|6|8|10|12|20|100)\\+)*"
                                               "(([1-9]\\d*)?[dD](4|6|8|10|12|20|100))"
                                               "([\\+\\-]\\d+)?";

Errors validate_dice_string(const std::string& str) {
    static const std::regex dice_regex(dice_regex_cstr);
    Errors errors;
    if (!std::regex_match(str, dice_regex)) {
        errors.add_runtime_error(RuntimeError::Code::INVALID_ARGUMENT, fmt::format("Invalid dice \"{}\"", str));
    }
    return errors;
}

} // namespace dnd
