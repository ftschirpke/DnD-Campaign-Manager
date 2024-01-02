#include <dnd_config.hpp>

#include "spell_data.hpp"

#include <memory>
#include <regex>
#include <string>
#include <string_view>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/spell/spell_components_data.hpp>
#include <core/validation/spell/spell_type_data.hpp>

namespace dnd {

Errors validate_spell_nonrecursively(const Spell::Data& data) {
    DND_MEASURE_FUNCTION();
    Errors errors = validate_name_description_and_source(data);
    if (data.casting_time.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Casting time is empty");
    }
    if (data.range.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Range is empty");
    }
    if (data.duration.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Duration is empty");
    }
    if (data.classes.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Spell has no classes");
    }
    return errors;
}

Errors validate_spell_recursively(const Spell::Data& data) {
    DND_MEASURE_FUNCTION();
    Errors errors = validate_spell_nonrecursively(data);
    errors += validate_spell_components(data.components_data);
    errors += validate_spell_type(data.type_data);
    return errors;
}

} // namespace dnd
