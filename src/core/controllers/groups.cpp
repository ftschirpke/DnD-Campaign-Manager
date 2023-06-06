#include <dnd_config.hpp>

#include "groups.hpp"

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/utils/string_manipulation.hpp>

dnd::Groups::Groups() {
    data["abilities"] = std::unordered_set<std::string>(
        ability_cstrings_inorder.begin(), ability_cstrings_inorder.end()
    );

    std::string lowercase_skill;
    for (const auto& [skill, _] : skill_abilities) {
        lowercase_skill = string_lowercase_copy(skill);

        size_t idx = lowercase_skill.find('_');
        while (idx != std::string::npos) {
            lowercase_skill[idx] = ' ';
            idx = lowercase_skill.find('_');
        }
        data["skills"].insert(lowercase_skill);
    }
}

bool dnd::Groups::isStringGroup(const std::string& group_name) const { return data.contains(group_name); }

bool dnd::Groups::isChoosableGroup(const std::string& group_name) const { return choosables.contains(group_name); }

bool dnd::Groups::isPartOfGroup(const std::string& name, const std::string& group_name) const {
    if (isStringGroup(group_name)) {
        return std::find(data.at(group_name).cbegin(), data.at(group_name).cend(), name) != data.at(group_name).cend();
    } else if (isChoosableGroup(group_name)) {
        return choosables.at(group_name).contains(name);
    }
    return false;
}

std::string dnd::Groups::status() const {
    std::stringstream sstr;
    sstr << "=== Groups ===\n";
    sstr << "string-based groups parsed: " << data.size() << '\n';
    sstr << "choosable-based groups parsed: " << choosables.size() << '\n';
    return sstr.str();
}
