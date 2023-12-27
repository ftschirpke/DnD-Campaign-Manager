#include <dnd_config.hpp>

#include "dice.hpp"

#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>

#include <fmt/format.h>

#include <core/exceptions/validation_exceptions.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>

namespace dnd {

static DiceType int_to_dice_type(int number) {
    switch (number) {
        case 4:
            return DiceType::D4;
        case 6:
            return DiceType::D6;
        case 8:
            return DiceType::D8;
        case 10:
            return DiceType::D10;
        case 12:
            return DiceType::D12;
        case 20:
            return DiceType::D20;
        case 100:
            return DiceType::D100;
        default:
            throw std::invalid_argument("No such dice exist.");
    };
}

static DiceType string_to_dice_type(const std::string& lowercase_str) {
    if (lowercase_str == "d4") {
        return DiceType::D4;
    } else if (lowercase_str == "d6") {
        return DiceType::D6;
    } else if (lowercase_str == "d8") {
        return DiceType::D8;
    } else if (lowercase_str == "d10") {
        return DiceType::D10;
    } else if (lowercase_str == "d12") {
        return DiceType::D12;
    } else if (lowercase_str == "d20") {
        return DiceType::D20;
    } else if (lowercase_str == "d100") {
        return DiceType::D100;
    } else {
        throw std::invalid_argument("No such dice exist.");
    }
}


Dice Dice::single_from_int(int dice_number) {
    return Dice(std::map<DiceType, int>{{int_to_dice_type(dice_number), 1}});
}

Dice Dice::single_from_int_with_modifier(int dice_number, int modifier) {
    return Dice(std::map<DiceType, int>{{int_to_dice_type(dice_number), 1}}, modifier);
}

Dice Dice::multi_from_int(int dice_number, int dice_count) {
    return Dice(std::map<DiceType, int>{{int_to_dice_type(dice_number), dice_count}});
}

Dice Dice::multi_from_int_with_modifier(int dice_number, int dice_count, int modifier) {
    return Dice(std::map<DiceType, int>{{int_to_dice_type(dice_number), dice_count}}, modifier);
}

Dice Dice::from_string(std::string&& str) {
    string_lowercase_inplace(str);
    std::map<DiceType, int> dice_counts;
    int modifier = 0;
    size_t current_index = 0;
    size_t d_index = str.find('d');
    size_t plus_index;
    if (d_index == std::string::npos) {
        throw invalid_data(fmt::format("Invalid dice string '{}': no 'd' (or 'D') found.", str));
    }
    do {
        int count = 1;
        if (d_index > current_index) {
            count = std::stoi(str.substr(current_index, d_index - current_index));
        }
        plus_index = str.find('+', d_index);
        if (plus_index == std::string::npos) {
            size_t minus_index = str.find('-', d_index);
            if (minus_index != std::string::npos) {
                DiceType dice_type = string_to_dice_type(str.substr(d_index, minus_index - d_index));
                dice_counts[dice_type] += count;
                modifier = std::stoi(str.substr(minus_index));
                return Dice(dice_counts, modifier);
            }
            DiceType dice_type = string_to_dice_type(str.substr(d_index));
            dice_counts[dice_type] += count;
            return Dice(dice_counts);
        }
        DiceType dice_type = string_to_dice_type(str.substr(d_index, plus_index - d_index));
        dice_counts[dice_type] += count;
        current_index = plus_index + 1;
        d_index = str.find('d', current_index);
    } while (d_index != std::string::npos);

    if (str.find('+', current_index) != std::string::npos) {
        throw invalid_data(fmt::format("Invalid dice string '{}': please put the modifier at the end", str));
    }
    modifier = std::stoi(str.substr(current_index));
    return Dice(dice_counts, modifier);
}

Dice Dice::create(DiceData&& data) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create dice from invalid data.");
    }
    return from_string(std::move(data.str));
}


Dice::Dice(std::map<DiceType, int> dice_counts) : dice_counts(dice_counts), modifier(0) {
    if (std::any_of(dice_counts.begin(), dice_counts.end(), [](const auto& pair) { return pair.second < 0; })) {
        throw std::invalid_argument("Dice counts cannot be negative.");
    }
}

Dice::Dice(std::map<DiceType, int> dice_counts, int modifier) : dice_counts(dice_counts), modifier(modifier) {}

int Dice::min_value() const noexcept {
    int min_value = modifier;
    for (const auto& [dice_type, dice_count] : dice_counts) {
        min_value += dice_count;
    }
    return min_value;
}

int Dice::max_value() const noexcept {
    int max_value = modifier;
    for (const auto& [dice_type, dice_count] : dice_counts) {
        max_value += static_cast<int>(dice_type) * dice_count;
    }
    return max_value;
}

bool Dice::value_is_possible(int value) const noexcept { return value >= min_value() && value <= max_value(); }

std::string Dice::to_string() const noexcept {
    std::string str;
    bool first = true;
    for (const auto& [dice_type, dice_count] : dice_counts) {
        if (first) {
            first = false;
        } else {
            str += '+';
        }
        str += fmt::format("{}d{}", dice_count, static_cast<int>(dice_type));
    }
    if (modifier > 0) {
        str += fmt::format("+{}", modifier);
    } else if (modifier < 0) {
        str += fmt::format("{}", modifier);
    }
    return str;
}

} // namespace dnd
