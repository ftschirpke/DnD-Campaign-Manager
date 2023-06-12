#include <dnd_config.hpp>

#include "character_race_data.hpp"

#include <vector>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>

dnd::Errors dnd::CharacterRaceData::validate() const {
    Errors errors;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate());
    }
    if (features_data.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character race has no features."
        );
    }
    return errors;
}

dnd::Errors dnd::CharacterRaceData::validate_relations(const ContentHolder* content) const {
    Errors errors;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate_relations(content));
    }
    return errors;
}
