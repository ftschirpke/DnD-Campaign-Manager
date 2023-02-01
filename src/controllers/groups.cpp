#include "dnd_config.hpp"

#include "groups.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "basic_mechanics/abilities.hpp"
#include "basic_mechanics/skills.hpp"

dnd::Groups::Groups() {
    data["abilities"] = std::unordered_set<std::string>(ability_strings_inorder.begin(), ability_strings_inorder.end());

    std::string lowercase_skill;
    for (const auto& [skill, _] : skill_abilities) {
        lowercase_skill = skill;
        auto tolower = [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); };
        std::transform(lowercase_skill.cbegin(), lowercase_skill.cend(), lowercase_skill.begin(), tolower);
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

void dnd::Groups::printStatus() const {
    std::cout << "=== Groups ===\n";
    std::cout << "string-based groups parsed: " << data.size() << '\n';
    std::cout << "choosable-based groups parsed: " << choosables.size() << '\n';
}
