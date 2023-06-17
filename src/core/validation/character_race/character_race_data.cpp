#include <dnd_config.hpp>

#include "character_race_data.hpp"

#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>

dnd::Errors dnd::CharacterRaceData::validate() const {
    Errors errors;
    std::unordered_set<std::string> unique_feature_names;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate());
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Character class has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    if (features_data.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character race has no features."
        );
    }
    return errors;
}

dnd::Errors dnd::CharacterRaceData::validate_relations(const ContentHolder& content) const {
    Errors errors;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate_relations(content));
        if (content.features.contains(name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Feature has duplicate name \"{}\".", name)
            );
        }
    }
    return errors;
}
