#include <dnd_config.hpp>

#include "abilities.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>

namespace dnd {

Ability string_to_ability(const std::string& ability_str) {
    for (size_t i = 0; i < 6; ++i) {
        if (ability_cstrings_inorder[i] == ability_str) {
            return abilities_inorder[i];
        }
    }
    throw std::invalid_argument("The ability \"" + ability_str + "\" does not exist.");
}

std::string ability_to_string(Ability ability) {
    switch (ability) {
        case Ability::STRENGTH:
            return "STR";
        case Ability::DEXTERITY:
            return "DEX";
        case Ability::CONSTITUTION:
            return "CON";
        case Ability::INTELLIGENCE:
            return "INT";
        case Ability::WISDOM:
            return "WIS";
        case Ability::CHARISMA:
            return "CHA";
    }
    assert(false);
    return "";
}

bool is_ability(std::string_view attribute_name) {
    return std::find(ability_cstrings_inorder.cbegin(), ability_cstrings_inorder.cend(), attribute_name)
           != ability_cstrings_inorder.cend();
}

bool is_ability(const std::string& attribute_name) {
    return std::find(ability_cstrings_inorder.cbegin(), ability_cstrings_inorder.cend(), attribute_name)
           != ability_cstrings_inorder.cend();
}

} // namespace dnd
