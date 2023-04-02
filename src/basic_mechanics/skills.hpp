#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include "dnd_config.hpp"

#include <array>
#include <utility>

namespace dnd {

// the names of the skills (in CAPS_SNAKECASE) paired with their respective abilities (in 3-letter representation)
inline constexpr std::array<std::pair<const char*, const char*>, 18> skill_abilities = {
    std::pair("ACROBATICS", "DEX"),      std::pair("ANIMAL_HANDLING", "WIS"), std::pair("ARCANA", "INT"),
    std::pair("ATHLETICS", "STR"),       std::pair("DECEPTION", "CHA"),       std::pair("HISTORY", "INT"),
    std::pair("INSIGHT", "WIS"),         std::pair("INTIMIDATION", "CHA"),    std::pair("INVESTIGATION", "INT"),
    std::pair("MEDICINE", "WIS"),        std::pair("NATURE", "INT"),          std::pair("PERCEPTION", "WIS"),
    std::pair("PERFORMANCE", "CHA"),     std::pair("PERSUASION", "CHA"),      std::pair("RELIGION", "INT"),
    std::pair("SLEIGHT_OF_HAND", "DEX"), std::pair("STEALTH", "DEX"),         std::pair("SURVIVAL", "WIS"),
};

} // namespace dnd

#endif // SKILLS_HPP_
