#ifndef DICE_HPP_
#define DICE_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>

#include <tl/expected.hpp>

#include <core/errors/errors.hpp>

namespace dnd {

enum class DiceType {
    D4 = 4,
    D6 = 6,
    D8 = 8,
    D10 = 10,
    D12 = 12,
    D20 = 20,
    D100 = 100,
};

class Dice {
public:
    static tl::expected<Dice, Errors> single_from_int(int dice_number);
    static tl::expected<Dice, Errors> single_from_int_with_modifier(int dice_number, int modifier);
    static tl::expected<Dice, Errors> multi_from_int(int dice_number, int dice_count);
    static tl::expected<Dice, Errors> multi_from_int_with_modifier(
        int dice_number, int dice_count, int modifier
    );
    static tl::expected<Dice, Errors> from_string(const std::string& str);
    static tl::expected<Dice, Errors> from_string(std::string&& str);
    static tl::expected<Dice, Errors> from_dice_count_map(std::map<DiceType, int>&& dice_counts);
    static tl::expected<Dice, Errors> from_dice_count_map_with_modifier(
        std::map<DiceType, int>&& dice_counts, int modifier
    );

    int min_value() const;
    int max_value() const;
    bool value_is_possible(int value) const;

    std::string to_string() const;
private:
    Dice(std::map<DiceType, int>&& dice_counts, int modifier);

    std::map<DiceType, int> dice_counts;
    int modifier;
};

} // namespace dnd

#endif // DICE_HPP_
