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

namespace dnd {

static Errors validate_subclass_raw_nonrecursively(const SubclassData& data) {
    Errors errors;
    std::unordered_set<std::string> unique_feature_names;
    for (const ClassFeatureData& feature_data : data.features_data) {
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Character class has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    if (data.features_data.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Character subclass has no features."
        );
    }
    if (data.class_name.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Character subclass has no class name."
        );
    }
    return errors;
}

static Errors validate_subclass_relations_nonrecursively(const SubclassData& data, const Content& content) {
    Errors errors;
    if (content.get_subclasses().contains(data.name)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Subclass has duplicate name \"{}\".", data.name)
        );
    }
    for (const ClassFeatureData& feature_data : data.features_data) {
        if (content.get_class_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }
    OptCRef<Class> class_optional = content.get_classes().get(data.class_name);
    if (!class_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND,
            fmt::format("Character class '{}' does not exist.", data.class_name)
        );
    } else if (data.spellcasting_data.is_spellcaster && class_optional.value().get().has_spellcasting()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_RELATION,
            fmt::format(
                "Character class '{}' has spellcasting. Its subclass '{}' cannot have spellcasting as well.",
                data.class_name, data.name
            )
        );
    }
    return errors;
}

Errors validate_subclass_nonrecursively_for_content(const SubclassData& data, const Content& content) {
    Errors errors = validate_subclass_raw_nonrecursively(data);
    errors += validate_subclass_relations_nonrecursively(data, content);
    return errors;
}

Errors validate_subclass_recursively_for_content(const SubclassData& data, const Content& content) {
    Errors errors = validate_subclass_nonrecursively_for_content(data, content);
    for (const ClassFeatureData& feature_data : data.features_data) {
        errors += validate_class_feature_recursively_for_content(feature_data, content);
    }
    errors += validate_spellcasting(data.spellcasting_data);
    return errors;
}

} // namespace dnd
