#include <dnd_config.hpp>

#include "dice.hpp"

#include <stdexcept>
#include <string>

namespace dnd {

int dice_to_int(Dice dice_type) { return static_cast<int>(dice_type); }

Dice int_to_dice(int number) {
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
        case 100:
            return Dice::D100;
        default:
            throw std::invalid_argument("No such dice exist.");
    };
}

std::string dice_to_string(Dice dice_type) { return 'd' + std::to_string(dice_to_int(dice_type)); }

Dice string_to_dice(const std::string& str) {
    std::string dice_str = str;
    if (dice_str[0] == 'D') {
        dice_str[0] = 'd';
    }
    if (dice_str == "d4") {
        return Dice::D4;
    } else if (dice_str == "d6") {
        return Dice::D6;
    } else if (dice_str == "d8") {
        return Dice::D8;
    } else if (dice_str == "d10") {
        return Dice::D10;
    } else if (dice_str == "d12") {
        return Dice::D12;
    } else if (dice_str == "d20") {
        return Dice::D20;
    } else if (dice_str == "d100") {
        return Dice::D100;
    } else {
        throw std::invalid_argument("No such dice exist.");
    }
}

bool value_is_possible_for(int value, Dice dice_type) { return value >= 1 && value <= dice_to_int(dice_type); }

} // namespace dnd
