#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include "dnd_config.hpp"

#include <map>
#include <string>

namespace dnd {

const std::map<std::string, std::string> skill_abilities = {
    {"ACROBATICS", "DEX"},    {"ANIMAL_HANDLING", "WIS"}, {"ARCANA", "INT"},   {"ATHLETICS", "STR"},
    {"DECEPTION", "CHA"},     {"HISTORY", "INT"},         {"INSIGHT", "WIS"},  {"INTIMIDATION", "CHA"},
    {"INVESTIGATION", "INT"}, {"MEDICINE", "WIS"},        {"NATURE", "INT"},   {"PERCEPTION", "WIS"},
    {"PERFORMANCE", "CHA"},   {"PERSUASION", "CHA"},      {"RELIGION", "INT"}, {"SLEIGHT_OF_HAND", "DEX"},
    {"STEALTH", "DEX"},       {"SURVIVAL", "WIS"},
};

} // namespace dnd

#endif // SKILLS_HPP_
