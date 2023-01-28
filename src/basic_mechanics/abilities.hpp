#ifndef ABILITIES_HPP_
#define ABILITIES_HPP_

#include "dnd_config.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

namespace dnd {

enum Ability {
    STRENGTH,
    DEXTERITY,
    CONSTITUTION,
    INTELLIGENCE,
    WISDOM,
    CHARISMA,
};

const std::array<Ability, 6> abilities_inorder = {
    Ability::STRENGTH,     Ability::DEXTERITY, Ability::CONSTITUTION,
    Ability::INTELLIGENCE, Ability::WISDOM,    Ability::CHARISMA,
};

const std::array<std::string, 6> ability_strings_inorder = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};

const std::unordered_map<std::string, Ability> ability_strings_mapping = {
    {"STR", Ability::STRENGTH},     {"DEX", Ability::DEXTERITY}, {"CON", Ability::CONSTITUTION},
    {"INT", Ability::INTELLIGENCE}, {"WIS", Ability::WISDOM},    {"CHA", Ability::CHARISMA},
};

inline Ability stringToAbility(const std::string& ability_str) {
    try {
        return ability_strings_mapping.at(ability_str);
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("The ability \"" + ability_str + "\" does not exist.");
    }
}

inline std::string abilityToString(Ability ability) {
    switch (ability) {
        case STRENGTH:
            return "STR";
        case DEXTERITY:
            return "DEX";
        case CONSTITUTION:
            return "CON";
        case INTELLIGENCE:
            return "INT";
        case WISDOM:
            return "WIS";
        case CHARISMA:
            return "CHA";
    }
}

inline bool isAbility(std::string_view attribute_name) {
    return std::find(ability_strings_inorder.cbegin(), ability_strings_inorder.cend(), attribute_name)
           != ability_strings_inorder.cend();
}

} // namespace dnd

#endif // ABILITIES_HPP_
