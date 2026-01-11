#include <dnd_config.hpp>

#include "dice.hpp"

#include <cassert>
#include <map>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/utils/string_manipulation.hpp>

namespace dnd {

static std::expected<DiceType, Errors> int_to_dice_type(int number) {
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
    };
    Errors errors(RuntimeError(
        RuntimeError::Code::INVALID_ARGUMENT,
        fmt::format("Invalid dice number '{}' - must be 4, 6, 8, 10, 12, 20, or 100", number)
    ));
    return std::unexpected(errors);
}

static std::expected<DiceType, Errors> string_to_dice_type(const std::string& lowercase_str) {
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
        Errors errors(RuntimeError(
            RuntimeError::Code::INVALID_ARGUMENT,
            fmt::format("Invalid dice type '{}' - must be d4, d6, d8, d10, d12, d20, or d100", lowercase_str)
        ));
        return std::unexpected(errors);
    }
}

static const char* dice_type_to_string(DiceType dice_type) {
    switch (dice_type) {
        case DiceType::D4:
            return "d4";
        case DiceType::D6:
            return "d6";
        case DiceType::D8:
            return "d8";
        case DiceType::D10:
            return "d10";
        case DiceType::D12:
            return "d12";
        case DiceType::D20:
            return "d20";
        case DiceType::D100:
            return "d100";
    };
    std::unreachable();
}


std::expected<Dice, Errors> Dice::single_from_int(int dice_number) {
    std::expected<DiceType, Errors> dice_type = int_to_dice_type(dice_number);
    if (dice_type.has_value()) {
        return Dice::from_dice_count_map(std::map<DiceType, int>{{dice_type.value(), 1}});
    } else {
        return std::unexpected(dice_type.error());
    }
}

std::expected<Dice, Errors> Dice::single_from_int_with_modifier(int dice_number, int modifier) {
    std::expected<DiceType, Errors> dice_type = int_to_dice_type(dice_number);
    if (dice_type.has_value()) {
        return Dice::from_dice_count_map_with_modifier(std::map<DiceType, int>{{dice_type.value(), 1}}, modifier);
    } else {
        return std::unexpected(dice_type.error());
    }
}

std::expected<Dice, Errors> Dice::multi_from_int(int dice_number, int dice_count) {
    std::expected<DiceType, Errors> dice_type = int_to_dice_type(dice_number);
    if (dice_type.has_value()) {
        return Dice::from_dice_count_map(std::map<DiceType, int>{{dice_type.value(), dice_count}});
    } else {
        return std::unexpected(dice_type.error());
    }
}

std::expected<Dice, Errors> Dice::multi_from_int_with_modifier(int dice_number, int dice_count, int modifier) {
    std::expected<DiceType, Errors> dice_type = int_to_dice_type(dice_number);
    if (dice_type.has_value()) {
        return Dice::from_dice_count_map_with_modifier(
            std::map<DiceType, int>{{dice_type.value(), dice_count}}, modifier
        );
    } else {
        return std::unexpected(dice_type.error());
    }
}

std::expected<Dice, Errors> Dice::from_string(const std::string& str) { return Dice::from_string(std::string(str)); }

std::expected<Dice, Errors> Dice::from_string(std::string&& str) {
    string_lowercase_inplace(str);
    std::map<DiceType, int> dice_counts;
    int modifier = 0;
    size_t current_index = 0;
    size_t d_index = str.find('d');
    size_t plus_index;
    if (d_index == std::string::npos) {
        Errors errors(RuntimeError(
            RuntimeError::Code::INVALID_ARGUMENT, fmt::format("Invalid dice string '{}': no 'd' (or 'D') found.", str)
        ));
        return std::unexpected(errors);
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
                std::expected<DiceType, Errors> dice_type = string_to_dice_type(
                    str.substr(d_index, minus_index - d_index)
                );
                if (!dice_type.has_value()) {
                    return std::unexpected(dice_type.error());
                }
                dice_counts[dice_type.value()] += count;
                modifier = std::stoi(str.substr(minus_index));
                return Dice::from_dice_count_map_with_modifier(std::move(dice_counts), modifier);
            }
            std::expected<DiceType, Errors> dice_type = string_to_dice_type(str.substr(d_index));
            if (!dice_type.has_value()) {
                return std::unexpected(dice_type.error());
            }
            dice_counts[dice_type.value()] += count;
            return Dice::from_dice_count_map(std::move(dice_counts));
        }
        std::expected<DiceType, Errors> dice_type = string_to_dice_type(str.substr(d_index, plus_index - d_index));
        if (!dice_type.has_value()) {
            return std::unexpected(dice_type.error());
        }
        dice_counts[dice_type.value()] += count;
        current_index = plus_index + 1;
        d_index = str.find('d', current_index);
    } while (d_index != std::string::npos);

    if (str.find('+', current_index) != std::string::npos) {
        Errors errors(RuntimeError(
            RuntimeError::Code::INVALID_ARGUMENT,
            fmt::format("Invalid dice string '{}': please put the modifier at the end", str)
        ));
        return std::unexpected(errors);
    }
    modifier = std::stoi(str.substr(current_index));
    return Dice::from_dice_count_map_with_modifier(std::move(dice_counts), modifier);
}

std::expected<Dice, Errors> Dice::from_dice_count_map(std::map<DiceType, int>&& dice_counts) {
    return Dice::from_dice_count_map_with_modifier(std::move(dice_counts), 0);
}

std::expected<Dice, Errors> Dice::from_dice_count_map_with_modifier(
    std::map<DiceType, int>&& dice_counts, int modifier
) {
    Errors errors;
    for (const auto& [dice_type, dice_count] : dice_counts) {
        if (dice_count < 0) {
            std::expected<const char*, Errors> dice_type_str = dice_type_to_string(dice_type);
            if (!dice_type_str.has_value()) {
                return std::unexpected(dice_type_str.error());
            }
            errors.add_runtime_error(
                RuntimeError::Code::INVALID_ARGUMENT,
                fmt::format(
                    "Invalid dice count '{}' for dice type '{}': must be non-negative", dice_count,
                    dice_type_str.value()
                )
            );
        }
    }
    if (!errors.ok()) {
        return std::unexpected(errors);
    }
    return Dice(std::move(dice_counts), modifier);
}

Dice::Dice(std::map<DiceType, int>&& dice_counts, int modifier) : dice_counts(dice_counts), modifier(modifier) {}

int Dice::min_value() const {
    int min_value = modifier;
    for (const auto& [dice_type, dice_count] : dice_counts) {
        min_value += dice_count;
    }
    return min_value;
}

int Dice::max_value() const {
    int max_value = modifier;
    for (const auto& [dice_type, dice_count] : dice_counts) {
        max_value += static_cast<int>(dice_type) * dice_count;
    }
    return max_value;
}

bool Dice::value_is_possible(int value) const { return value >= min_value() && value <= max_value(); }

std::string Dice::to_string() const {
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
