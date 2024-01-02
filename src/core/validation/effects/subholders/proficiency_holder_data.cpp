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

static Errors validate_proficiency_holder_raw(const ProficiencyHolder::Data& data) {
    Errors errors;
    errors += string_set_validate(data.armor, "Armor");
    errors += string_set_validate(data.weapons, "Weapons");
    errors += string_set_validate(data.tools, "Tools");
    errors += string_set_validate(data.skills, "Skills");
    errors += string_set_validate(data.saving_throws, "Saving throws");
    errors += string_set_validate(data.languages, "Languages");
    errors += string_set_validate(data.senses, "Senses");

    for (const std::string& skill : data.skills) {
        if (!is_skill(skill)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Skill proficiencies contain '{}' which is not a valid skill.", skill)
            );
        }
    }
    for (const std::string& saving_throw_ability : data.saving_throws) {
        if (!is_ability(saving_throw_ability)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format(
                    "Saving throw proficiencies contain '{}' which is not a valid ability.", saving_throw_ability
                )
            );
        }
    }
    return errors;
}

static Errors string_group_set_validate_relations(
    const std::set<std::string>& string_group_set, const char* set_name, const char* group_name, const Content& content
) {
    Errors errors;
    for (const std::string& str_item : string_group_set) {
        if (!content.get_groups().is_part_of_group(str_item, group_name)
            && !content.get_groups().is_subgroup(str_item, group_name)) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format("'{}' ({}) is neither element or subgroup of the {} group.", str_item, set_name, group_name)
            );
        }
    }
    return errors;
}

static Errors validate_proficiency_holder_relations(const ProficiencyHolder::Data& data, const Content& content) {
    Errors errors;
    errors += string_group_set_validate_relations(data.armor, "armor", "armor", content);
    errors += string_group_set_validate_relations(data.weapons, "weapons", "weapons", content);
    errors += string_group_set_validate_relations(data.tools, "tools", "tools", content);
    errors += string_group_set_validate_relations(data.languages, "languages", "languages", content);
    errors += string_group_set_validate_relations(data.senses, "senses", "senses", content);
    return errors;
}

Errors validate_proficiency_holder_for_content(const ProficiencyHolder::Data& data, const Content& content) {
    Errors errors = validate_proficiency_holder_raw(data);
    errors += validate_proficiency_holder_relations(data, content);
    return errors;
}

} // namespace dnd
