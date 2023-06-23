#ifndef CHARACTER_PROGRESSION_HPP_
#define CHARACTER_PROGRESSION_HPP_

#include <dnd_config.hpp>

namespace dnd {

/**
 * @brief Converts a character level to the amount of xp needed to reach that level
 * @param level the character level to convert
 * @return the amount of xp needed to reach the given level
 * @throws std::invalid_argument if the given level is not a valid character level i.e. not between 1 and 20 (inclusive)
 */
int level_to_xp(int level);
/**
 * @brief Converts an amount of xp to the character level that amount of xp corresponds to
 * @param xp the amount of xp to convert
 * @return the character level that the given amount of xp corresponds to
 * @throws std::invalid_argument if the given xp amount is negative
 */
int xp_to_level(int xp);

} // namespace dnd

#endif // CHARACTER_PROGRESSION_HPP_
