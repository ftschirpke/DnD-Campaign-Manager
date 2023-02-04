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

// the basic 6 abilities
enum Ability {
    STRENGTH,
    DEXTERITY,
    CONSTITUTION,
    INTELLIGENCE,
    WISDOM,
    CHARISMA,
};

// the abilities in order
const std::array<Ability, 6> abilities_inorder = {
    Ability::STRENGTH,     Ability::DEXTERITY, Ability::CONSTITUTION,
    Ability::INTELLIGENCE, Ability::WISDOM,    Ability::CHARISMA,
};

// the 3-letter strings for the abilities in order
const std::array<std::string, 6> ability_strings_inorder = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};

// the 3-letter strings mapped to their respective ability
const std::unordered_map<std::string, Ability> ability_strings_mapping = {
    {"STR", Ability::STRENGTH},     {"DEX", Ability::DEXTERITY}, {"CON", Ability::CONSTITUTION},
    {"INT", Ability::INTELLIGENCE}, {"WIS", Ability::WISDOM},    {"CHA", Ability::CHARISMA},
};

/**
 * @brief Given the 3-letter string representation of an ability, returns the ability value.
 * @param ability_str the 3-letter string representing an ability (e.g. "DEX")
 * @return the ability the 3-letter string represents
 * @throws std::invalid_argument if the string doesn't represent any of the 6 abilities
 */
inline Ability stringToAbility(const std::string& ability_str) {
    try {
        return ability_strings_mapping.at(ability_str);
    } catch (const std::out_of_range& e) {
        DND_UNUSED(e);
        throw std::invalid_argument("The ability \"" + ability_str + "\" does not exist.");
    }
}

/**
 * @brief Given a certain ability, return the 3-letter string representation
 * @param ability the ability
 * @return the 3-letter string representation of the ability
 */
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

/**
 * @brief Checks whether the given 3-letter string represents an ability
 * @param attribute_name the 3-letter string representation of an ability as string_view
 * @return "true" if string represents an ability, "no" otherwise
 */
inline bool isAbility(std::string_view attribute_name) {
    return std::find(ability_strings_inorder.cbegin(), ability_strings_inorder.cend(), attribute_name)
           != ability_strings_inorder.cend();
}


/**
 * @brief Checks whether the given 3-letter string represents an ability
 * @param attribute_name the 3-letter string representation of an ability
 * @return "true" if string represents an ability, "no" otherwise
 */
inline bool isAbility(const std::string& attribute_name) {
    return std::find(ability_strings_inorder.cbegin(), ability_strings_inorder.cend(), attribute_name)
           != ability_strings_inorder.cend();
}

} // namespace dnd

#endif // ABILITIES_HPP_
