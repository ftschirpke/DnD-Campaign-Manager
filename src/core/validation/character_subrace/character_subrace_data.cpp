#include <dnd_config.hpp>

#include "character_subrace_data.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/feature/feature_data.hpp>

std::unique_ptr<dnd::ValidationData> dnd::CharacterSubraceData::pack() const {
    return std::make_unique<CharacterSubraceData>(*this);
}

dnd::Errors dnd::CharacterSubraceData::validate() const {
    Errors errors;
    std::unordered_set<std::string> unique_feature_names;
    for (const auto& feature_data : features_data) {
        errors += feature_data.validate();
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
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character subrace has no features."
        );
    }
    return errors;
}

dnd::Errors dnd::CharacterSubraceData::validate_relations(const Content& content) const {
    Errors errors;
    if (content.get_character_subraces().contains(name)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, fmt::format("Subrace has duplicate name \"{}\".", name)
        );
    }
    for (const auto& feature_data : features_data) {
        errors += feature_data.validate_relations(content);
        if (content.get_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Feature has duplicate name \"{}\".", name)
            );
        }
    }
    if (!content.get_character_races().contains(race_name)) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, this, fmt::format("Character race '{}' does not exist.", race_name)
        );
    } else if (!content.get_character_races().get(race_name).has_subraces()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_RELATION, this,
            fmt::format("Character race '{}' does not have subraces.", race_name)
        );
    }
    return errors;
}
