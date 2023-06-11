#include <dnd_config.hpp>

#include "proficiency_holder_data.hpp"

#include <set>
#include <string>
#include <utility>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::ProficiencyHolderData::ProficiencyHolderData(const dnd::ValidationData* parent) noexcept
    : ValidationSubdata(parent) {}

static dnd::Errors string_set_validate(
    const std::set<std::string>& string_set, const dnd::ValidationData* parent, const char* set_name
) {
    dnd::Errors errors;
    for (const auto& str_item : string_set) {
        if (str_item.empty()) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format("{} set contains an empty string.", set_name)
            );
        }
    }
    return errors;
}

dnd::Errors dnd::ProficiencyHolderData::validate() const {
    Errors errors;
    errors.merge(string_set_validate(armor, parent, "Armor"));
    errors.merge(string_set_validate(weapons, parent, "Weapons"));
    errors.merge(string_set_validate(tools, parent, "Tools"));
    errors.merge(string_set_validate(skills, parent, "Skills"));
    errors.merge(string_set_validate(saving_throws, parent, "Saving throws"));
    errors.merge(string_set_validate(languages, parent, "Languages"));
    errors.merge(string_set_validate(senses, parent, "Senses"));
    return errors;
}

static dnd::Errors string_group_set_validate_relations(
    const std::set<std::string>& string_group_set, const dnd::ValidationData* parent, const char* set_name,
    const char* group_name, const dnd::ContentHolder* content
) {
    dnd::Errors errors;
    for (const auto& str_item : string_group_set) {
        if (!content->groups.is_part_of_string_group(str_item, group_name)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format("'{}' ({}) is not part of the {} group.", str_item, set_name, group_name)
            );
        }
    }
    return errors;
}

dnd::Errors dnd::ProficiencyHolderData::validate_relations(const dnd::ContentHolder* content) const {
    Errors errors;
    errors.merge(string_group_set_validate_relations(armor, parent, "armor", "armor", content));
    errors.merge(string_group_set_validate_relations(weapons, parent, "weapons", "weapons", content));
    errors.merge(string_group_set_validate_relations(tools, parent, "tools", "tools", content));
    errors.merge(string_group_set_validate_relations(skills, parent, "skills", "skills", content));
    errors.merge(string_group_set_validate_relations(saving_throws, parent, "saving_throws", "abilities", content));
    errors.merge(string_group_set_validate_relations(languages, parent, "languages", "languages", content));
    return errors;
}

bool dnd::ProficiencyHolderData::empty() const noexcept {
    return armor.empty() && weapons.empty() && tools.empty() && skills.empty() && saving_throws.empty()
           && languages.empty() && senses.empty();
}
