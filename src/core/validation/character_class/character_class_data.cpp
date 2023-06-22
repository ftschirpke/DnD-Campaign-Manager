#include <dnd_config.hpp>

#include "character_class_data.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>
#include <core/validation/character_class/important_levels_data.hpp>
#include <core/validation/character_class/spellcasting/spellcasting_data.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::CharacterClassData::CharacterClassData() noexcept
    : ValidationData(), spellcasting_data(this), features_data(), subclass_feature_name(), hit_dice_data(this),
      important_levels_data(this) {}

std::unique_ptr<dnd::ValidationData> dnd::CharacterClassData::pack() const {
    return std::make_unique<CharacterClassData>(*this);
}

dnd::Errors dnd::CharacterClassData::validate() const {
    Errors errors = ValidationData::validate();
    errors += spellcasting_data.validate();

    bool has_subclass_feature = false;
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
    errors += hit_dice_data.validate();
    errors += important_levels_data.validate();
    return errors;
}

dnd::Errors dnd::CharacterClassData::validate_relations(const dnd::ContentHolder& content) const {
    Errors errors;
    for (const auto& feature_data : features_data) {
        errors += feature_data.validate_relations(content);
        if (content.get_features().contains(name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Feature has duplicate name \"{}\".", name)
            );
        }
    }
    return errors;
}
