#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include "dnd_config.hpp"

#include <array>
#include <utility>

namespace dnd {

// the names of the skills (in CAPS_SNAKECASE) paired with their respective abilities (in 3-letter representation)
constexpr std::array<std::pair<const char*, const char*>, 18> skill_abilities = {
    std::make_pair("ACROBATICS", "DEX"),    std::make_pair("ANIMAL_HANDLING", "WIS"),
    std::make_pair("ARCANA", "INT"),        std::make_pair("ATHLETICS", "STR"),
    std::make_pair("DECEPTION", "CHA"),     std::make_pair("HISTORY", "INT"),
    std::make_pair("INSIGHT", "WIS"),       std::make_pair("INTIMIDATION", "CHA"),
    std::make_pair("INVESTIGATION", "INT"), std::make_pair("MEDICINE", "WIS"),
    std::make_pair("NATURE", "INT"),        std::make_pair("PERCEPTION", "WIS"),
    std::make_pair("PERFORMANCE", "CHA"),   std::make_pair("PERSUASION", "CHA"),
    std::make_pair("RELIGION", "INT"),      std::make_pair("SLEIGHT_OF_HAND", "DEX"),
    std::make_pair("STEALTH", "DEX"),       std::make_pair("SURVIVAL", "WIS"),
};

} // namespace dnd

#endif // SKILLS_HPP_
