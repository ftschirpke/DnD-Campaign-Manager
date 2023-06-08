#include <dnd_config.hpp>

#include "dice.hpp"

#include <stdexcept>
#include <string>

int dnd::dice_to_int(dnd::Dice dice_type) { return static_cast<int>(dice_type); }

dnd::Dice dnd::int_to_dice(int number) {
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

std::string dnd::dice_to_string(dnd::Dice dice_type) { return 'd' + std::to_string(dice_to_int(dice_type)); }

dnd::Dice dnd::string_to_dice(const std::string& str) {
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
