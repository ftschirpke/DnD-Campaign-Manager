#ifndef DICE_HPP_
#define DICE_HPP_

#include "dnd_config.hpp"

#include <stdexcept>
#include <string>

namespace dnd {

/**
 * @brief An enum for the types of typical tabletop dice.
 */
enum Dice {
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
inline int diceToInt(Dice dice_type) { return static_cast<int>(dice_type); }

/**
 * @brief Returns the type of dice for a given number
 * @param number the maximum value of the type of dice
 * @return the type of dice that has the number as the maximum value
 * @throws std::invalid_argument if no such dice exist (i.e. number is not 4, 6, 8, 10, 12, or 20)
 */
inline Dice diceFromInt(int number) {
    switch (number) {
        case 4:
            return Dice::D4;
        case 6:
            return Dice::D6;
        case 8:
            return Dice::D8;
        case 10:
            return Dice::D10;
        case 12:
            return Dice::D12;
        case 20:
            return Dice::D20;
        default:
            throw std::invalid_argument("No such dice exist.");
    };
}

/**
 * @brief Returns the string representation for a type of dice
 * @param dice_type a type tabletop dice
 * @return the string representation for that type of dice
 */
inline std::string diceToString(Dice dice_type) { return 'd' + std::to_string(diceToInt(dice_type)); }

/**
 * @brief Returns a type of dice given its string representation
 * @param dice_type a string representation for a type of dice (e.g. "d6")
 * @return the type of tabletop dice that this string represents
 * @throws std::invalid_argument if no such dice exist (i.e. is not d4, d6, d8, d10, d12, or d20)
 */
inline Dice diceFromString(const std::string& str) {
    if (str == "d4") {
        return Dice::D4;
    } else if (str == "d6") {
        return Dice::D6;
    } else if (str == "d8") {
        return Dice::D8;
    } else if (str == "d10") {
        return Dice::D10;
    } else if (str == "d12") {
        return Dice::D12;
    } else if (str == "d20") {
        return Dice::D20;
    } else {
        throw std::invalid_argument("No such dice exist.");
    }
}

} // namespace dnd

#endif // DICE_HPP_
