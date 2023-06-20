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
#include <core/validation/spell/spell_components_data.hpp>
#include <core/validation/spell/spell_type_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::SpellData::SpellData() noexcept : components_data(this), type_data(this) {}

dnd::Errors dnd::SpellData::validate() const {
    DND_MEASURE_FUNCTION();
    Errors errors = ValidationData::validate();
    errors += components_data.validate();
    errors += type_data.validate();
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
