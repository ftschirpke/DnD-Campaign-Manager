#include <dnd_config.hpp>

#include "character_basis_data.hpp"

#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
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

dnd::Errors dnd::CharacterBasisData::validate_relations(const dnd::Content& content) const {
    Errors errors;
    OptRef<const CharacterRace> race_optional = content.get_character_races().get(race_name);
    if (!race_optional.has_value()) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, fmt::format("Race '{}' does not exist.", race_name)
        );
    } else if (!subrace_name.empty()) {
        const CharacterRace& race = race_optional.value();
        if (!race.has_subraces()) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_RELATION, parent,
                fmt::format("Race '{}' does not have subraces.", race_name)
            );
        }
        OptRef<const CharacterSubrace> subrace_optional = content.get_character_subraces().get(subrace_name);
        if (!subrace_optional.has_value()) {
            errors.add_validation_error(
                ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format("Subrace '{}' does not exist.", subrace_name)
            );
        } else if (subrace_optional.value().get().get_race() != &race) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_RELATION, parent,
                fmt::format("Subrace '{}' is not a subrace of race '{}'.", subrace_name, race_name)
            );
        }
    } else if (race_optional.value().get().has_subraces()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_RELATION, parent, fmt::format("Race '{}' requires subraces.", race_name)
        );
    }

    OptRef<const CharacterClass> class_optional = content.get_character_classes().get(class_name);
    if (!class_optional.has_value()) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, parent, fmt::format("Class '{}' does not exist.", class_name)
        );
    } else if (!subclass_name.empty()) {
        const CharacterClass& cls = class_optional.value();
        OptRef<const CharacterSubclass> subclass_optional = content.get_character_subclasses().get(subclass_name);
        if (!subclass_optional.has_value()) {
            errors.add_validation_error(
                ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format("Subclass '{}' does not exist.", subclass_name)
            );
        } else if (subclass_optional.value().get().get_class() != &cls) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_RELATION, parent,
                fmt::format("Subclass '{}' is not a subclass of class '{}'.", subclass_name, class_name)
            );
        }
    }
    return errors;
}
