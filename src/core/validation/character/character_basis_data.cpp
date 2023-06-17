#include <dnd_config.hpp>

#include "character_basis_data.hpp"

#include <string>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::CharacterBasisData::CharacterBasisData(const dnd::ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::CharacterBasisData::validate() const {
    Errors errors;
    if (race_name.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Race name cannot be empty.");
    }
    if (class_name.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Class name cannot be empty."
        );
    }
    return errors;
}

dnd::Errors dnd::CharacterBasisData::validate_relations(const dnd::ContentHolder& content) const {
    Errors errors;
    if (!content.character_races.contains(race_name)) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, fmt::format("Race '{}' does not exist.", race_name)
        );
    } else if (!subrace_name.empty()) {
        if (!content.character_races.get(race_name).has_subraces()) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_RELATION, parent,
                fmt::format("Race '{}' does not have subraces.", race_name)
            );
        }
        if (!content.character_subraces.contains(subrace_name)) {
            errors.add_validation_error(
                ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format("Subrace '{}' does not exist.", subrace_name)
            );
        }
    } else if (content.character_races.get(race_name).has_subraces()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_RELATION, parent, fmt::format("Race '{}' requires subraces.", race_name)
        );
    }

    if (!content.character_classes.contains(class_name)) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, fmt::format("Class '{}' does not exist.", class_name)
        );
    }
    if (!subclass_name.empty() && !content.character_subclasses.contains(subclass_name)) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, fmt::format("Subclass '{}' does not exist.", subclass_name)
        );
    }
    return errors;
}
