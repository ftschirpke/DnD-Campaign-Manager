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

/**
 * @brief An enum for the basic 6 abilities
 */
enum Ability {
    STRENGTH,
    DEXTERITY,
    CONSTITUTION,
    INTELLIGENCE,
    WISDOM,
    CHARISMA,
};

// the abilities in order
inline constexpr std::array<Ability, 6> abilities_inorder = {
    Ability::STRENGTH,     Ability::DEXTERITY, Ability::CONSTITUTION,
    Ability::INTELLIGENCE, Ability::WISDOM,    Ability::CHARISMA,
};

// the 3-letter c-style strings for the abilities in order
inline constexpr std::array<const char*, 6> ability_cstrings_inorder = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};

/**
 * @brief Given the 3-letter string representation of an ability, returns the ability value.
 * @param ability_str the 3-letter string representing an ability (e.g. "DEX")
 * @return the ability the 3-letter string represents
 * @throws std::invalid_argument if the string doesn't represent any of the 6 abilities
 */
inline Ability stringToAbility(const std::string& ability_str) {
    for (size_t i = 0; i < 6; ++i) {
        if (ability_cstrings_inorder[i] == ability_str) {
            return abilities_inorder[i];
        }
    }
    throw std::invalid_argument("The ability \"" + ability_str + "\" does not exist.");
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
    return std::find(ability_cstrings_inorder.cbegin(), ability_cstrings_inorder.cend(), attribute_name)
           != ability_cstrings_inorder.cend();
}

/**
 * @brief Checks whether the given 3-letter string represents an ability
 * @param attribute_name the 3-letter string representation of an ability
 * @return "true" if string represents an ability, "no" otherwise
 */
inline bool isAbility(const std::string& attribute_name) {
    return std::find(ability_cstrings_inorder.cbegin(), ability_cstrings_inorder.cend(), attribute_name)
           != ability_cstrings_inorder.cend();
}

} // namespace dnd

#endif // ABILITIES_HPP_
