#include <dnd_config.hpp>

#include "decision_data.hpp"

#include <string>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>

dnd::DecisionData::DecisionData(const dnd::CharacterData* parent) noexcept
    : ValidationSubdata(parent), choice_name(), effects_data(parent) {}

dnd::Errors dnd::DecisionData::validate() const {
    Errors errors = effects_data.validate();
    if (parent == nullptr) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Decision has no character that it belongs to."
        );
    } else if (choice_name.empty()) {
        errors.add_validation_error(ValidationErrorCode::MISSING_ATTRIBUTE, parent, "Decision's choice name is empty.");
    }
    return errors;
}
