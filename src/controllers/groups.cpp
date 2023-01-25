#include "dnd_config.hpp"

#include "groups.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "basic_mechanics/abilities.hpp"
#include "basic_mechanics/skills.hpp"

dnd::Groups::Groups() {
    data["abilities"] = std::unordered_set<std::string>(ability_strings_inorder.begin(), ability_strings_inorder.end());

    std::string lowercase_skill;
    for (const auto& [skill, _] : skill_abilities) {
        lowercase_skill = skill;
        std::transform(lowercase_skill.cbegin(), lowercase_skill.cend(), lowercase_skill.begin(), ::tolower);
        size_t idx = lowercase_skill.find('_');
        while (idx != std::string::npos) {
            lowercase_skill[idx] = ' ';
            idx = lowercase_skill.find('_');
        }
        data["skills"].insert(lowercase_skill);
    }
}

void dnd::Groups::printStatus() const {
    std::cout << "=== Groups ===\n";
    std::cout << "string-based groups parsed: " << data.size() << '\n';
    std::cout << "choosable-based groups parsed: " << choosables.size() << '\n';
}
