#include <dnd_config.hpp>

#include "riv_holder_validation.hpp"

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>

namespace dnd {

static Errors string_set_validate(const std::set<std::string>& string_set, const char* set_name) {
    Errors errors;
    for (const std::string& str_item : string_set) {
        if (str_item.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("{} set contains an empty string.", set_name)
            );
        }
    }
    return errors;
}

static Errors validate_riv_holder_raw(const RIVHolder::Data& data) {
    Errors errors;
    errors += string_set_validate(data.damage_resistances, "Damage resistances");
    errors += string_set_validate(data.damage_immunities, "Damage immunities");
    errors += string_set_validate(data.damage_vulnerabilities, "Damage vulnerabilities");
    errors += string_set_validate(data.condition_immunities, "Condition immunities");
    return errors;
}

static Errors string_group_set_validate_relations(
    const std::set<std::string>& string_group_set, const char* set_name, const char* group_name, const Content& content
) {
    Errors errors;
    for (const std::string& str_item : string_group_set) {
        if (!content.get_groups().is_member_of_group(str_item, group_name)) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format("Invalid value in {}: '{}' is not part of the {} group.", str_item, set_name, group_name)
            );
        }
    }
    return errors;
}

static Errors validate_riv_holder_relations(const RIVHolder::Data& data, const Content& content) {
    Errors errors;
    errors += string_group_set_validate_relations(
        data.damage_resistances, "damage resistances", "damage types", content
    );
    errors += string_group_set_validate_relations(data.damage_immunities, "damage immunities", "damage types", content);
    errors += string_group_set_validate_relations(
        data.damage_vulnerabilities, "damage vulnerabilities", "damage types", content
    );
    errors += string_group_set_validate_relations(
        data.condition_immunities, "condition immunities", "conditions", content
    );
    return errors;
}

Errors validate_riv_holder_for_content(const RIVHolder::Data& data, const Content& content) {
    Errors errors = validate_riv_holder_raw(data);
    errors += validate_riv_holder_relations(data, content);
    return errors;
}

} // namespace dnd
