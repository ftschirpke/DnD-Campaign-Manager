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
#include <core/validation/basic_mechanics/dice_validation.hpp>
#include <core/validation/class/important_levels_data.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

ClassData::ClassData() noexcept
    : ValidationData(), spellcasting_data(std::make_shared<ValidationData>(this)), features_data(),
      subclass_feature_name(), hit_dice_str(), important_levels_data(std::make_shared<ValidationData>(this)) {}

std::unique_ptr<ValidationData> ClassData::pack() const { return std::make_unique<ClassData>(*this); }

static Errors validate_class_raw_nonrecursively(const ClassData& data) {
    Errors errors = validate_name_description_and_source(data);

    bool has_subclass_feature = false;
    std::unordered_set<std::string> unique_feature_names;
    for (const ClassFeatureData& feature_data : data.features_data) {
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
                fmt::format("Character class has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
        if (feature_data.name == data.subclass_feature_name) {
            has_subclass_feature = true;
        }
    }
    if (data.features_data.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(), "Character class has no features."
        );
    }
    if (!has_subclass_feature) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
            fmt::format(
                "The declared subclass feature \"{}\" is not a feature of the character class.",
                data.subclass_feature_name
            )
        );
    }
    return errors;
}

static Errors validate_class_relations_nonrecursively(const ClassData& data, const Content& content) {
    Errors errors;
    if (content.get_classes().contains(data.name)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
            fmt::format("Class has duplicate name \"{}\".", data.name)
        );
    }
    for (const ClassFeatureData& feature_data : data.features_data) {
        if (content.get_class_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }
    return errors;
}

Errors validate_class_recursively(const ClassData& data, const Content& content) {
    Errors errors = validate_class_raw_nonrecursively(data);
    errors += validate_spellcasting(data.spellcasting_data);
    for (const ClassFeatureData& feature_data : data.features_data) {
        errors += validate_class_feature_recursively_for_content(feature_data, content);
    }
    errors += validate_dice_string(data.hit_dice_str);
    errors += validate_important_levels(data.important_levels_data);
    return errors;
}

} // namespace dnd
