#include <dnd_config.hpp>

#include "spell_data.hpp"

#include <memory>
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

namespace dnd {

SpellData::SpellData() noexcept : components_data(this), type_data(this) {}

std::unique_ptr<ValidationData> SpellData::pack() const { return std::make_unique<SpellData>(*this); }

Errors SpellData::validate() const {
    DND_MEASURE_FUNCTION();

    Errors errors = validate_nonrecursively();
    errors += components_data.validate();
    errors += type_data.validate();
    return errors;
}

Errors SpellData::validate_nonrecursively() const {
    DND_MEASURE_FUNCTION();
    Errors errors = ValidationData::validate();
    if (casting_time.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, this, "Casting time is empty");
    }
    if (range.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, this, "Range is empty");
    }
    if (duration.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, this, "Duration is empty");
    }
    if (classes.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, this, "Spell has no classes");
    }
    return errors;
}

} // namespace dnd
