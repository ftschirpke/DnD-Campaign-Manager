#ifndef ATTRIBUTE_NAMES_HPP_
#define ATTRIBUTE_NAMES_HPP_

#include <array>
#include <string>

#include <fmt/format.h>

namespace dnd::attributes {

inline constexpr const char* MAXIMUM_HP = "MAX_HP";
inline constexpr const char* ARMOR_CLASS = "AC";
inline constexpr const char* SPEED = "SPEED";
inline constexpr const char* PROFICIENCY_BONUS = "PB";
inline constexpr const char* INITIATIVE = "INITIATIVE";

inline constexpr const char* STRENGTH = "str";
inline constexpr const char* DEXTERITY = "dex";
inline constexpr const char* CONSTITUTION = "con";
inline constexpr const char* INTELLIGENCE = "int";
inline constexpr const char* WISDOM = "wis";
inline constexpr const char* CHARISMA = "cha";

inline constexpr const std::array<const char*, 6> ABILITIES = {STRENGTH,     DEXTERITY, CONSTITUTION,
                                                               INTELLIGENCE, WISDOM,    CHARISMA};

inline constexpr const char* ACROBATICS = "ACROBATICS";
inline constexpr const char* ANIMAL_HANDLING = "ANIMAL_HANDLING";
inline constexpr const char* ARCANA = "ARCANA";
inline constexpr const char* ATHLETICS = "ATHLETICS";
inline constexpr const char* DECEPTION = "DECEPTION";
inline constexpr const char* HISTORY = "HISTORY";
inline constexpr const char* INSIGHT = "INSIGHT";
inline constexpr const char* INTIMIDATION = "INTIMIDATION";
inline constexpr const char* INVESTIGATION = "INVESTIGATION";
inline constexpr const char* MEDICINE = "MEDICINE";
inline constexpr const char* NATURE = "NATURE";
inline constexpr const char* PERCEPTION = "PERCEPTION";
inline constexpr const char* PERFORMANCE = "PERFORMANCE";
inline constexpr const char* PERSUASION = "PERSUASION";
inline constexpr const char* RELIGION = "RELIGION";
inline constexpr const char* SLEIGHT_OF_HAND = "SLEIGHT_OF_HAND";
inline constexpr const char* STEALTH = "STEALTH";
inline constexpr const char* SURVIVAL = "SURVIVAL";

inline std::string ability_maximum(const char* ability) { return fmt::format("{}_MAX", ability); }
inline std::string ability_maximum(const std::string& ability) { return fmt::format("{}_MAX", ability); }

inline std::string ability_modifier(const char* ability) { return fmt::format("{}_MOD", ability); }
inline std::string ability_modifier(const std::string& ability) { return fmt::format("{}_MOD", ability); }

inline std::string ability_saving_throw(const char* ability) { return fmt::format("{}_SAVE", ability); }
inline std::string ability_saving_throw(const std::string& ability) { return fmt::format("{}_SAVE", ability); }

} // namespace dnd::attributes

#endif // ATTRIBUTE_NAMES_HPP_
