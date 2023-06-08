#ifndef ABILITIES_HPP_
#define ABILITIES_HPP_

#include <dnd_config.hpp>

#include <array>
#include <string>
#include <string_view>

namespace dnd {

/**
 * @brief An enum for the basic 6 abilities
 */
enum class Ability {
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
Ability string_to_ability(const std::string& ability_str);

/**
 * @brief Given a certain ability, return the 3-letter string representation
 * @param ability the ability
 * @return the 3-letter string representation of the ability
 */
std::string ability_to_string(Ability ability);

/**
 * @brief Checks whether the given 3-letter string represents an ability
 * @param attribute_name the 3-letter string representation of an ability as string_view
 * @return "true" if string represents an ability, "false" otherwise
 */
bool is_ability(std::string_view attribute_name);

/**
 * @brief Checks whether the given 3-letter string represents an ability
 * @param attribute_name the 3-letter string representation of an ability
 * @return "true" if string represents an ability, "false" otherwise
 */
bool is_ability(const std::string& attribute_name);

} // namespace dnd

#endif // ABILITIES_HPP_
