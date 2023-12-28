#ifndef DICE_HPP_
#define DICE_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>

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

class DiceData;

class Dice {
public:
    using Data = DiceData;

    /**
     * @brief Constructs a dice object with one die of the given type
     * @param dice_type the type of die to construct the dice object with
     * @return the constructed dice object
     * @throws invalid_data if the dice type is invalid
     */
    static Dice single_from_int(int dice_number);
    /**
     * @brief Constructs a dice object with one die of the given type and a modifier
     * @param dice_type the type of die to construct the dice object with
     * @param modifier the modifier to add to the dice roll
     * @return the constructed dice object
     * @throws invalid_data if the dice type is invalid
     */
    static Dice single_from_int_with_modifier(int dice_number, int modifier);
    /**
     * @brief Constructs a dice object with multiple dice of the given type
     * @param dice_type the type of die to construct the dice object with
     * @param dice_count the number of dice to construct the dice object with
     * @return the constructed dice object
     * @throws invalid_data if the dice type is invalid
     */
    static Dice multi_from_int(int dice_number, int dice_count);
    /**
     * @brief Constructs a dice object with multiple dice of the given type and a modifier
     * @param dice_type the type of die to construct the dice object with
     * @param dice_count the number of dice to construct the dice object with
     * @param modifier the modifier to add to the dice roll
     * @return the constructed dice object
     * @throws invalid_data if the dice type is invalid
     */
    static Dice multi_from_int_with_modifier(int dice_number, int dice_count, int modifier);
    /**
     * @brief Constructs a dice object from the given string
     * @param str the string to construct the dice object from
     * @return the constructed dice object
     * @throw invalid_data if the string is invalid
     */
    static Dice from_string(std::string&& str);

    /**
     * @brief Constructs a dice object from the given data
     * @param data the data to construct the dice object from
     * @return the constructed dice object
     * @throw invalid_data if the data is invalid
     */
    static Dice create(Data&& data);

    Dice(std::map<DiceType, int> dice_counts);
    Dice(std::map<DiceType, int> dice_counts, int modifier);

    int min_value() const noexcept;
    int max_value() const noexcept;
    bool value_is_possible(int value) const noexcept;

    std::string to_string() const noexcept;
private:
    std::map<DiceType, int> dice_counts;
    int modifier;
};

} // namespace dnd

#endif // DICE_HPP_
