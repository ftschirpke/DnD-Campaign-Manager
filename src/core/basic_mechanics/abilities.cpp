#include <dnd_config.hpp>

#include "abilities.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <optional>
#include <string>
#include <string_view>

#include <core/attribute_names.hpp>

namespace dnd {

std::optional<Ability> ability_from_string(const std::string& ability_str) {
    for (size_t i = 0; i < 6; ++i) {
        if (attributes::ABILITIES[i] == ability_str) {
            return abilities_inorder[i];
        }
    }
    return std::nullopt;
}

std::string ability_name(Ability ability) { return std::string(ability_cstr_name(ability)); }

const char* ability_cstr_name(Ability ability) {
    switch (ability) {
        case Ability::STRENGTH:
            return attributes::STRENGTH;
        case Ability::DEXTERITY:
            return attributes::DEXTERITY;
        case Ability::CONSTITUTION:
            return attributes::CONSTITUTION;
        case Ability::INTELLIGENCE:
            return attributes::INTELLIGENCE;
        case Ability::WISDOM:
            return attributes::WISDOM;
        case Ability::CHARISMA:
            return attributes::CHARISMA;
    }
    assert(false);
    return "";
}

bool is_ability(std::string_view attribute_name) {
    return std::find(attributes::ABILITIES.cbegin(), attributes::ABILITIES.cend(), attribute_name)
           != attributes::ABILITIES.cend();
}

bool is_ability(const std::string& attribute_name) {
    return std::find(attributes::ABILITIES.cbegin(), attributes::ABILITIES.cend(), attribute_name)
           != attributes::ABILITIES.cend();
}

} // namespace dnd
