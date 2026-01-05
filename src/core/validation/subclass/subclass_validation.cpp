#include <dnd_config.hpp>

#include "subclass_validation.hpp"

#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/validation/effects_provider/subclass_feature_validation.hpp>
#include <core/validation/spellcasting/spellcasting_validation.hpp>

namespace dnd {

static Errors validate_subclass_raw_nonrecursively(const Subclass::Data& data) {
    Errors errors;
    if (data.short_name.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Character subclass has short name."
        );
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
    if (data.class_key.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Character subclass has no class key."
        );
    }
    return errors;
}

static Errors validate_subclass_relations_nonrecursively(const Subclass::Data& data, const Content& content) {
    Errors errors;
    if (content.get_subclasses().contains(data.get_key())) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Subclass has duplicate key \"{}\".", data.get_key())
        );
    }
    for (const SubclassFeature::Data& feature_data : data.features_data) {
        if (content.get_class_features().contains(feature_data.get_key())) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Feature has duplicate key \"{}\".", feature_data.get_key())
            );
        }
    }

    OptCRef<Class> class_optional = content.get_classes().get(data.class_key);
    if (!class_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND,
            fmt::format("Character class '{}' does not exist.", data.class_key)
        );
    } else if (data.class_name != class_optional.value().get().get_name()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_RELATION,
            fmt::format("Character class '{}' (by key) does not have given name '{}'.", data.class_key, data.class_name)
        );
    } else if (data.spellcasting_data.is_spellcaster && class_optional.value().get().has_spellcasting()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_RELATION,
            fmt::format(
                "Character class '{}' has spellcasting. Its subclass '{}' cannot have spellcasting as well.",
                data.class_key, data.name
            )
        );
    }
    return errors;
}

Errors validate_subclass_nonrecursively_for_content(const Subclass::Data& data, const Content& content) {
    Errors errors = validate_subclass_raw_nonrecursively(data);
    errors += validate_subclass_relations_nonrecursively(data, content);
    return errors;
}

Errors validate_subclass_recursively_for_content(const Subclass::Data& data, const Content& content) {
    Errors errors = validate_subclass_nonrecursively_for_content(data, content);
    for (const SubclassFeature::Data& feature_data : data.features_data) {
        errors += validate_subclass_feature_recursively_for_content(feature_data, content);
    }
    errors += validate_spellcasting(data.spellcasting_data);
    return errors;
}

} // namespace dnd
