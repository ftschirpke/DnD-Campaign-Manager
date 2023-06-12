#include <dnd_config.hpp>

#include "character_subrace_data.hpp"

#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/feature/feature_data.hpp>

dnd::Errors dnd::CharacterSubraceData::validate() const {
    Errors errors;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate());
    }
    if (features_data.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character subrace has no features."
        );
    }
    return errors;
}

dnd::Errors dnd::CharacterSubraceData::validate_relations(const ContentHolder* content) const {
    Errors errors;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate_relations(content));
    }
    if (!content->character_races.contains(race_name)) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, this, fmt::format("Character race '{}' does not exist.", race_name)
        );
    } else if (!content->character_races.get(race_name).has_subraces()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_RELATION, this,
            fmt::format("Character race '{}' does not have subraces.", race_name)
        );
    }
    return errors;
}
