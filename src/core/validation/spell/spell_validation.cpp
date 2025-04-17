#include <dnd_config.hpp>

#include "spell_validation.hpp"

#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/spell/spell_type_validation.hpp>

namespace dnd {

Errors validate_spell_nonrecursively(const Spell::Data& data) {
    DND_MEASURE_FUNCTION();
    Errors errors = validate_name_description_and_source(data);
    if (data.casting_time.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Spell casting time of '{}' is empty", data.name)
        );
    }
    if (data.range.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, fmt::format("Spell range of '{}' is empty", data.name)
        );
    }
    if (data.duration.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, fmt::format("Spell duration of '{}' is empty", data.name)
        );
    }
    return errors;
}

Errors validate_spell_recursively(const Spell::Data& data) {
    DND_MEASURE_FUNCTION();
    Errors errors = validate_spell_nonrecursively(data);
    errors += validate_spell_type(data.type_data);
    return errors;
}

} // namespace dnd
