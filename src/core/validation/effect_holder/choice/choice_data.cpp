#include <dnd_config.hpp>

#include "choice_data.hpp"

#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>

dnd::ChoiceData::ChoiceData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::ChoiceData::validate() const {
    Errors errors;
    if (attribute_name.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Choice has emtpy attribute name"
        );
    }
    if (amount <= 0) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, "Choice has non-positive amount"
        );
    }
    return errors;
}

dnd::Errors dnd::ChoiceData::validate_relations(const ContentHolder& content) const {
    Errors errors;
    if (!group_names.empty()) {
        for (const auto& group_name : group_names) {
            if (!content.groups.is_string_group(group_name)) {
                errors.add_validation_error(
                    ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                    fmt::format("Choice has group name '{}' which is not a group", group_name)
                );
            }
        }
    } else if (explicit_choices.empty()) {
        if (!content.groups.is_string_group(attribute_name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Choice has no group(s) or explicit choices defined but the attribute name '{}' is not a group",
                    attribute_name
                )
            );
        }
    }
    return Errors();
}
