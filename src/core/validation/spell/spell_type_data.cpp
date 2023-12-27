#include <dnd_config.hpp>

#include "spell_type_data.hpp"

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

static constexpr const char* spell_type_regex_cstr = "(1st|2nd|3rd|[4-9]th)-level "
                                                     "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                     "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                     "( \\(ritual\\))?|"
                                                     "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                     "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                     " cantrip";

SpellTypeData::SpellTypeData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

Errors SpellTypeData::validate() const {
    DND_MEASURE_FUNCTION();
    static const std::regex spell_type_regex(spell_type_regex_cstr);
    Errors errors;
    if (!std::regex_match(str, spell_type_regex)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, fmt::format("Invalid spell type \"{}\"", str)
        );
    }
    return errors;
}

} // namespace dnd
