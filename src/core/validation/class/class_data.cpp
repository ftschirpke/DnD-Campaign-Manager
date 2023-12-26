#include <dnd_config.hpp>

#include "class_data.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>
#include <core/validation/class/important_levels_data.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::ClassData::ClassData() noexcept
    : ValidationData(), spellcasting_data(this), features_data(), subclass_feature_name(), hit_dice_data(this),
      important_levels_data(this) {}

std::unique_ptr<dnd::ValidationData> dnd::ClassData::pack() const { return std::make_unique<ClassData>(*this); }

dnd::Errors dnd::ClassData::validate() const {
    Errors errors = ValidationData::validate();
    errors += spellcasting_data.validate();

    bool has_subclass_feature = false;
    std::unordered_set<std::string> unique_feature_names;
    for (const ClassFeatureData& feature_data : features_data) {
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

dnd::Errors dnd::ClassData::validate_relations(const dnd::Content& content) const {
    Errors errors;
    if (content.get_classes().contains(name)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, fmt::format("Class has duplicate name \"{}\".", name)
        );
    }
    for (const ClassFeatureData& feature_data : features_data) {
        errors += feature_data.validate_relations(content);
        if (content.get_class_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }
    return errors;
}