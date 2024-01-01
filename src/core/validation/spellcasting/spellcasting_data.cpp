#include <dnd_config.hpp>

#include "spellcasting_data.hpp"

#include <algorithm>
#include <array>
#include <string>

#include <fmt/format.h>

#include <core/basic_mechanics/abilities.hpp>
#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

SpellcastingData::SpellcastingData(std::shared_ptr<const ValidationData> parent) noexcept : ValidationSubdata(parent) {
    for (int& val : spells_known) {
        val = 0;
    }
    for (int& val : cantrips_known) {
        val = 0;
    }
    for (std::array<int, 20>& arr : spell_slots) {
        for (int& val : arr) {
            val = 0;
        }
    }
}

Errors validate_spellcasting(const SpellcastingData& data) {
    Errors errors;

    if (!data.is_spellcaster) {
        return errors;
    }
    if (!is_ability(data.ability)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
            fmt::format("The spellcasting ability '{}' is not a valid ability.", data.ability)
        );
    }

    if (data.is_spells_known_type && !data.preparation_spellcasting_type.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
            "The spellcasting cannot be a spells known spellcasting and preparation spellcasting at the same time."
        );
    } else if (data.is_spells_known_type) {
        for (int val : data.spells_known) {
            if (val < 0) {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
                    "The amount of spells known cannot be negative"
                );
                break;
            }
        }
    } else if (!data.preparation_spellcasting_type.empty()) {
        if (std::any_of(data.spells_known.begin(), data.spells_known.end(), [](int val) { return val != 0; })) {
            errors.add_validation_error(
                ValidationError::Code::MISSING_ATTRIBUTE, data.get_parent(),
                "The spells known must be empty for preparation spellcasting."
            );
        } else if (data.preparation_spellcasting_type != "full" && data.preparation_spellcasting_type != "half") {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
                fmt::format(
                    "The preparation spellcasting type '{}' is not a valid type (must be 'full' of 'half').",
                    data.preparation_spellcasting_type
                )
            );
        }
    } else {
        errors.add_validation_error(
            ValidationError::Code::MISSING_ATTRIBUTE, data.get_parent(),
            "The spellcasting must be either a spells known spellcasting or a preparation spellcasting."
        );
    }

    for (int val : data.cantrips_known) {
        if (val < 0) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
                "The amount of cantrips known cannot be negative"
            );
            break;
        }
    }

    int level = 1;
    for (const std::array<int, 20>& arr : data.spell_slots) {
        for (int val : arr) {
            if (val < 0) {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
                    fmt::format("The amount of spell slots of level {} cannot be negative", level)
                );
                break;
            }
        }
        ++level;
    }

    return errors;
}

} // namespace dnd
