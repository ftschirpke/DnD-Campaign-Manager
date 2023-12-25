#include <dnd_config.hpp>

#include "riv_holder_data.hpp"

#include <set>
#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::RIVHolderData::RIVHolderData(const dnd::ValidationData* parent) noexcept : ValidationSubdata(parent) {}

static dnd::Errors string_set_validate(
    const std::set<std::string>& string_set, const dnd::ValidationData* parent, const char* set_name
) {
    dnd::Errors errors;
    for (const std::string& str_item : string_set) {
        if (str_item.empty()) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format("{} set contains an empty string.", set_name)
            );
        }
    }
    return errors;
}

dnd::Errors dnd::RIVHolderData::validate() const {
    Errors errors;
    errors += string_set_validate(damage_resistances, parent, "Damage resistances");
    errors += string_set_validate(damage_immunities, parent, "Damage immunities");
    errors += string_set_validate(damage_vulnerabilities, parent, "Damage vulnerabilities");
    errors += string_set_validate(condition_immunities, parent, "Condition immunities");
    return errors;
}

static dnd::Errors string_group_set_validate_relations(
    const std::set<std::string>& string_group_set, const dnd::ValidationData* parent, const char* set_name,
    const char* group_name, const dnd::Content& content
) {
    dnd::Errors errors;
    for (const std::string& str_item : string_group_set) {
        if (!content.get_groups().is_part_of_group(str_item, group_name)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format("Invalid value in {}: '{}' is not part of the {} group.", str_item, set_name, group_name)
            );
        }
    }
    return errors;
}

dnd::Errors dnd::RIVHolderData::validate_relations(const dnd::Content& content) const {
    Errors errors;
    errors += string_group_set_validate_relations(
        damage_resistances, parent, "damage resistances", "damage types", content
    );
    errors += string_group_set_validate_relations(
        damage_immunities, parent, "damage immunities", "damage types", content
    );
    errors += string_group_set_validate_relations(
        damage_vulnerabilities, parent, "damage vulnerabilities", "damage types", content
    );
    errors += string_group_set_validate_relations(
        condition_immunities, parent, "condition immunities", "conditions", content
    );
    return errors;
}

bool dnd::RIVHolderData::empty() const noexcept {
    return damage_resistances.empty() && damage_immunities.empty() && damage_vulnerabilities.empty()
           && condition_immunities.empty();
}
