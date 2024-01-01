#include <dnd_config.hpp>

#include "riv_holder_data.hpp"

#include <set>
#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

RIVHolderData::RIVHolderData(std::shared_ptr<const ValidationData> parent) noexcept : ValidationSubdata(parent) {}

bool RIVHolderData::empty() const noexcept {
    return damage_resistances.empty() && damage_immunities.empty() && damage_vulnerabilities.empty()
           && condition_immunities.empty();
}

static Errors string_set_validate(
    const std::set<std::string>& string_set, std::shared_ptr<const ValidationData> parent, const char* set_name
) {
    Errors errors;
    for (const std::string& str_item : string_set) {
        if (str_item.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format("{} set contains an empty string.", set_name)
            );
        }
    }
    return errors;
}

static Errors validate_riv_holder_raw(const RIVHolderData& data) {
    Errors errors;
    errors += string_set_validate(data.damage_resistances, data.get_parent(), "Damage resistances");
    errors += string_set_validate(data.damage_immunities, data.get_parent(), "Damage immunities");
    errors += string_set_validate(data.damage_vulnerabilities, data.get_parent(), "Damage vulnerabilities");
    errors += string_set_validate(data.condition_immunities, data.get_parent(), "Condition immunities");
    return errors;
}

static Errors string_group_set_validate_relations(
    const std::set<std::string>& string_group_set, std::shared_ptr<const ValidationData> parent, const char* set_name,
    const char* group_name, const Content& content
) {
    Errors errors;
    for (const std::string& str_item : string_group_set) {
        if (!content.get_groups().is_part_of_group(str_item, group_name)) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND, parent,
                fmt::format("Invalid value in {}: '{}' is not part of the {} group.", str_item, set_name, group_name)
            );
        }
    }
    return errors;
}

static Errors validate_riv_holder_relations(const RIVHolderData& data, const Content& content) {
    Errors errors;
    errors += string_group_set_validate_relations(
        data.damage_resistances, data.get_parent(), "damage resistances", "damage types", content
    );
    errors += string_group_set_validate_relations(
        data.damage_immunities, data.get_parent(), "damage immunities", "damage types", content
    );
    errors += string_group_set_validate_relations(
        data.damage_vulnerabilities, data.get_parent(), "damage vulnerabilities", "damage types", content
    );
    errors += string_group_set_validate_relations(
        data.condition_immunities, data.get_parent(), "condition immunities", "conditions", content
    );
    return errors;
}

Errors validate_riv_holder_for_content(const RIVHolderData& data, const Content& content) {
    Errors errors = validate_riv_holder_raw(data);
    errors += validate_riv_holder_relations(data, content);
    return errors;
}

} // namespace dnd
