#ifndef DICE_FACTORY_HPP_
#define DICE_FACTORY_HPP_

#include <dnd_config.hpp>

#include <core/basic_mechanics/dice.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>

namespace dnd {

/**
 * @brief Constructs a dice object from the given data
 * @param data the data to construct the dice object from
 * @return the constructed dice object
 * @throw dnd::invalid_data if the data is invalid
 */
Dice create_dice(DiceData&& data);

} // namespace dnd

#endif // DICE_FACTORY_HPP_
