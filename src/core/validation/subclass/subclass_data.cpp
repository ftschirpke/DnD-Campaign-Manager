#include <dnd_config.hpp>

#include "subclass_data.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>

dnd::SubclassData::SubclassData() noexcept : ValidationData(), spellcasting_data(this), features_data(), class_name() {}

std::unique_ptr<dnd::ValidationData> dnd::SubclassData::pack() const { return std::make_unique<SubclassData>(*this); }

dnd::Errors dnd::SubclassData::validate() const {
    Errors errors;
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
    }
    if (features_data.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character subclass has no features."
        );
    }
    if (class_name.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character subclass has no class name."
        );
    }
    errors += spellcasting_data.validate();
    return errors;
}

dnd::Errors dnd::SubclassData::validate_relations(const Content& content) const {
    Errors errors;
    if (content.get_subclasses().contains(name)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, fmt::format("Subclass has duplicate name \"{}\".", name)
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
    OptCRef<Class> class_optional = content.get_classes().get(class_name);
    if (!class_optional.has_value()) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, this,
            fmt::format("Character class '{}' does not exist.", class_name)
        );
    } else if (spellcasting_data.is_spellcaster && class_optional.value().get().has_spellcasting()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_RELATION, this,
            fmt::format(
                "Character class '{}' has spellcasting. Its subclass '{}' cannot have spellcasting as well.",
                class_name, name
            )
        );
    }
    errors += spellcasting_data.validate_relations(content);
    return errors;
}