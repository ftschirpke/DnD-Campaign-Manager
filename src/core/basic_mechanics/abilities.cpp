#include <dnd_config.hpp>

#include "abilities.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <optional>
#include <string>
#include <string_view>

namespace dnd {

std::optional<Ability> ability_from_string(const std::string& ability_str) {
    for (size_t i = 0; i < 6; ++i) {
        if (ability_cstrings_inorder[i] == ability_str) {
            return abilities_inorder[i];
        }
    }
    return std::nullopt;
}

std::string ability_name(Ability ability) {
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
