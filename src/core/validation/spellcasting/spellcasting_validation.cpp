#include <dnd_config.hpp>

#include "spellcasting_validation.hpp"

#include <algorithm>
#include <array>
#include <string>

#include <fmt/format.h>

#include <core/basic_mechanics/abilities.hpp>
#include <core/errors/errors.hpp>

namespace dnd {

Errors validate_spellcasting(const Spellcasting::Data& data) {
    Errors errors;

    if (!data.is_spellcaster) {
        return errors;
    }
    if (!is_ability(data.ability)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("The spellcasting ability '{}' is not a valid ability.", data.ability)
        );
    }

    if (data.is_spells_known_type && !data.preparation_spellcasting_type.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            "The spellcasting cannot be a spells known spellcasting and preparation spellcasting at the same time."
        );
    } else if (data.is_spells_known_type) {
        for (int val : data.spells_known) {
            if (val < 0) {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "The amount of spells known cannot be negative"
                );
                break;
            }
        }
    } else if (!data.preparation_spellcasting_type.empty()) {
        if (std::any_of(data.spells_known.begin(), data.spells_known.end(), [](int val) { return val != 0; })) {
            errors.add_validation_error(
                ValidationError::Code::MISSING_ATTRIBUTE, "The spells known must be empty for preparation spellcasting."
            );
        } else if (data.preparation_spellcasting_type != "full" && data.preparation_spellcasting_type != "half"
                   && data.preparation_spellcasting_type != "subclass") {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format(
                    "The preparation spellcasting type '{}' is not a valid type (must be 'full', 'half', or "
                    "'subclass').",
                    data.preparation_spellcasting_type
                )
            );
        }
    } else {
        errors.add_validation_error(
            ValidationError::Code::MISSING_ATTRIBUTE,
            "The spellcasting must be either a spells known spellcasting or a preparation spellcasting."
        );
    }

    for (int val : data.cantrips_known) {
        if (val < 0) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "The amount of cantrips known cannot be negative"
            );
            break;
        }
    }

    int level = 1;
    for (const std::array<int, 20>& arr : data.spell_slots) {
        for (int val : arr) {
            if (val < 0) {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                    fmt::format("The number of spell slots of level {} cannot be negative", level)
                );
                break;
            }
        }
        ++level;
    }

    return errors;
}

} // namespace dnd
