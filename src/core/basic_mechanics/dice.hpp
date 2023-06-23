#ifndef DICE_HPP_
#define DICE_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

/**
 * @brief An enum for the types of typical tabletop dice.
 */
enum class Dice {
    D4 = 4,
    D6 = 6,
    D8 = 8,
    D10 = 10,
    D12 = 12,
    D20 = 20,
};

/**
 * @brief Returns the maximum value for a type of dice
 * @param dice_type a type tabletop dice
 * @return the maximum value for that type of dice
 */
int dice_to_int(Dice dice_type);

/**
 * @brief Returns the type of dice for a given number
 * @param number the maximum value of the type of dice
 * @return the type of dice that has the number as the maximum value
 * @throws std::invalid_argument if no such dice exist (i.e. number is not 4, 6, 8, 10, 12, or 20)
 */
Dice int_to_dice(int number);

/**
 * @brief Returns the string representation for a type of dice
 * @param dice_type a type tabletop dice
 * @return the string representation for that type of dice
 */
std::string dice_to_string(Dice dice_type);

/**
 * @brief Returns a type of dice given its string representation
 * @param dice_type a string representation for a type of dice (e.g. "d6")
 * @return the type of tabletop dice that this string represents
 * @throws std::invalid_argument if no such dice exist (i.e. is not d4, d6, d8, d10, d12, or d20)
 */
Dice string_to_dice(const std::string& str);

/**
 * @brief Returns whether a value is possible for a type of dice
 * @param value the value to check
 * @param dice_type the type of dice to check
 * @return "true" if the value is possible for the type of dice, "false" otherwise
 */
bool value_is_possible_for(int value, Dice dice_type);

} // namespace dnd

#endif // DICE_HPP_
