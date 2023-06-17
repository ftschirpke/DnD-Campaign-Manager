#include <dnd_config.hpp>

#include "character_subclass_data.hpp"

#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/feature/feature_data.hpp>

dnd::Errors dnd::CharacterSubclassData::validate() const {
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
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character subclass has no features."
        );
    }
    return errors;
}

dnd::Errors dnd::CharacterSubclassData::validate_relations(const ContentHolder& content) const {
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
    if (!content.character_classes.contains(class_name)) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, this,
            fmt::format("Character class '{}' does not exist.", class_name)
        );
    }
    return errors;
}
