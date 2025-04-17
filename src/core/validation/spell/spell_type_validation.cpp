#include <dnd_config.hpp>

#include "spell_type_validation.hpp"

#include <optional>

#include <fmt/format.h>

#include <constants.hpp>
#include <core/basic_mechanics/magic_schools.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/spell/spell_type.hpp>

namespace dnd {

Errors validate_spell_type(const SpellType::Data& data) {
    DND_MEASURE_FUNCTION();
    Errors errors;
    std::optional<MagicSchool> magic_school_opt = magic_school_from_char(data.magic_school_char);
    if (!magic_school_opt.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Invalid magic school '{}'", data.magic_school_char)
        );
    }
    if (data.level < 0 || data.level > MAX_SPELL_LEVEL) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Spell level must be 0 (cantrip) or between 1 and 9 - is {}", data.level)
        );
    }
    if (data.level == 0 && data.ritual) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Cantrips (spell level 0) cannot be rituals"
        );
    }
    return errors;
}

} // namespace dnd
