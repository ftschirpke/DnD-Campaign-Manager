#include <dnd_config.hpp>

#include "proficiency_holder_data.hpp"

#include <set>
#include <string>
#include <utility>

#include <fmt/format.h>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

ProficiencyHolderData::ProficiencyHolderData(const ValidationData* parent) noexcept
    : ValidationSubdata(parent) {}

static Errors string_set_validate(
    const std::set<std::string>& string_set, const ValidationData* parent, const char* set_name
) {
    Errors errors;
    for (const std::string& str_item : string_set) {
        if (str_item.empty()) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format("{} set contains an empty string.", set_name)
            );
        }
    }
    return errors;
}

Errors ProficiencyHolderData::validate() const {
    Errors errors;
    errors += string_set_validate(armor, parent, "Armor");
    errors += string_set_validate(weapons, parent, "Weapons");
    errors += string_set_validate(tools, parent, "Tools");
    errors += string_set_validate(skills, parent, "Skills");
    errors += string_set_validate(saving_throws, parent, "Saving throws");
    errors += string_set_validate(languages, parent, "Languages");
    errors += string_set_validate(senses, parent, "Senses");

    for (const std::string& skill : skills) {
        if (!is_skill(skill)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format("Skill proficiencies contain '{}' which is not a valid skill.", skill)
            );
        }
    }
    for (const std::string& saving_throw_ability : saving_throws) {
        if (!is_ability(saving_throw_ability)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent,
                fmt::format(
                    "Saving throw proficiencies contain '{}' which is not a valid ability.", saving_throw_ability
                )
            );
        }
    }
    return errors;
}

static Errors string_group_set_validate_relations(
    const std::set<std::string>& string_group_set, const ValidationData* parent, const char* set_name,
    const char* group_name, const Content& content
) {
    Errors errors;
    for (const std::string& str_item : string_group_set) {
        if (!content.get_groups().is_part_of_group(str_item, group_name)
            && !content.get_groups().is_subgroup(str_item, group_name)) {
            errors.add_validation_error(
                ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format("'{}' ({}) is neither element or subgroup of the {} group.", str_item, set_name, group_name)
            );
        }
    }
    return errors;
}

Errors ProficiencyHolderData::validate_relations(const Content& content) const {
    Errors errors;
    errors += string_group_set_validate_relations(armor, parent, "armor", "armor", content);
    errors += string_group_set_validate_relations(weapons, parent, "weapons", "weapons", content);
    errors += string_group_set_validate_relations(tools, parent, "tools", "tools", content);
    errors += string_group_set_validate_relations(languages, parent, "languages", "languages", content);
    errors += string_group_set_validate_relations(senses, parent, "senses", "senses", content);
    return errors;
}

bool ProficiencyHolderData::empty() const noexcept {
    return armor.empty() && weapons.empty() && tools.empty() && skills.empty() && saving_throws.empty()
           && languages.empty() && senses.empty();
}

} // namespace dnd
