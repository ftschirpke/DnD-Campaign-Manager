#include <dnd_config.hpp>

#include "spell_components_data.hpp"

#include <regex>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::SpellComponentsData::SpellComponentsData(const dnd::ValidationData* parent) : ValidationSubdata(parent) {}

dnd::Errors dnd::SpellComponentsData::validate() const {
    DND_MEASURE_FUNCTION();
    static const std::regex spell_components_regex(spell_components_regex_cstr);
    Errors errors;
    if (!std::regex_match(str, spell_components_regex)) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Invalid spell components");
    }
    return errors;
}
