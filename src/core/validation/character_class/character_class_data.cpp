#include <dnd_config.hpp>

#include "character_class_data.hpp"

#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>
#include <core/validation/character_class/important_levels_data.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::Errors dnd::CharacterClassData::validate() const {
    Errors errors = ValidationData::validate();
    bool has_subclass_feature = false;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate());
        if (feature_data.name == subclass_feature_name) {
            has_subclass_feature = true;
        }
    }
    if (features_data.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character class has no features."
        );
    }
    if (!has_subclass_feature) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
            fmt::format(
                "The declared subclass feature \"{}\" is not a feature of the character class.", subclass_feature_name
            )
        );
    }
    errors.merge(hit_dice_data.validate());
    errors.merge(important_levels_data.validate());
    return errors;
}

dnd::Errors dnd::CharacterClassData::validate_relations(const dnd::ContentHolder* content) const {
    Errors errors;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate_relations(content));
    }
    return errors;
}
