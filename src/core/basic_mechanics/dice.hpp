#ifndef DICE_HPP_
#define DICE_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>

#include <tl/expected.hpp>

#include <core/errors/errors.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>

namespace dnd {

/**
 * @brief An enum for the types of typical tabletop dice.
 */
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
    using Data = DiceData;

    static tl::expected<Dice, Errors> single_from_int(int dice_number) noexcept;
    static tl::expected<Dice, Errors> single_from_int_with_modifier(int dice_number, int modifier) noexcept;
    static tl::expected<Dice, Errors> multi_from_int(int dice_number, int dice_count) noexcept;
    static tl::expected<Dice, Errors> multi_from_int_with_modifier(
        int dice_number, int dice_count, int modifier
    ) noexcept;
    static tl::expected<Dice, Errors> from_string(std::string&& str) noexcept;
    static tl::expected<Dice, Errors> from_dice_count_map(std::map<DiceType, int>&& dice_counts) noexcept;
    static tl::expected<Dice, Errors> from_dice_count_map_with_modifier(
        std::map<DiceType, int>&& dice_counts, int modifier
    ) noexcept;

    static CreateResult<Dice> create(Data&& data) noexcept;

    int min_value() const noexcept;
    int max_value() const noexcept;
    bool value_is_possible(int value) const noexcept;

    std::string to_string() const noexcept;
private:
    Dice(std::map<DiceType, int>&& dice_counts, int modifier) noexcept;

    std::map<DiceType, int> dice_counts;
    int modifier;
};

} // namespace dnd

#endif // DICE_HPP_
