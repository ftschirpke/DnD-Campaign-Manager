#include <dnd_config.hpp>

#include "spell_components_data.hpp"

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

static constexpr const char* spell_components_regex_cstr = "((1st|2nd|3rd|[4-9]th)-level "
                                                           "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                           "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                           "( \\(ritual\\))?)|("
                                                           "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                           "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                           " cantrip)";

dnd::SpellComponentsData::SpellComponentsData(const dnd::ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::SpellComponentsData::validate() const {
    DND_MEASURE_FUNCTION();
    static const std::regex spell_components_regex(spell_components_regex_cstr);
    Errors errors;
    if (!std::regex_match(str, spell_components_regex)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, fmt::format("Invalid spell components \"{}\"", str)
        );
    }
    return errors;
}
