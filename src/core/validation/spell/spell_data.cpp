#include <dnd_config.hpp>

#include "spell_data.hpp"

#include <regex>
#include <string>
#include <string_view>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/validation_data.hpp>

static constexpr const char* spell_components_regex_cstr = "((1st|2nd|3rd|[4-9]th)-level "
                                                           "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                           "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                           "( \\(ritual\\))?)|("
                                                           "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                           "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                           " cantrip)";

static constexpr const char*
    spell_type_regex_cstr = "(V, S, M \\(.+\\)|V, S|V, M \\(.*\\)|S, M \\(.+\\)|V|S|M \\(.+\\))";

dnd::Errors dnd::SpellData::validate() const {
    DND_MEASURE_FUNCTION();
    Errors errors = ValidationData::validate();
    errors.merge(components_data.validate());
    errors.merge(type_data.validate());
    if (casting_time.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Casting time is empty");
    }
    if (range.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Range is empty");
    }
    if (duration.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Duration is empty");
    }
    return errors;
}
