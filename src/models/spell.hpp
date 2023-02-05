#ifndef SPELL_HPP_
#define SPELL_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace dnd {

/**
 * @brief An enum for the types of magic or so called magic schools
 */
enum MagicSchool {
    // abjuration - wards and defensive spells
    ABJURATION,
    // conjuration - summoning, creation, and teleportation
    CONJURATION,
    // divination - knowledge, providing answers, and identification of objects
    DIVINATION,
    // enchantment - affecting mind or mental state of friends and foes
    ENCHANTMENT,
    // evocation - high-damage combat spells and healing
    EVOCATION,
    // illusion - deception and manipulation
    ILLUSION,
    // necromancy - manipulation of life itself
    NECROMANCY,
    // transmutation - physically alter the form of an object
    TRANSMUTATION
};

// the magic schools mapped to their names
const std::unordered_map<std::string, MagicSchool> magic_schools = {
    {"abjuration", MagicSchool::ABJURATION}, {"conjuration", MagicSchool::CONJURATION},
    {"divination", MagicSchool::DIVINATION}, {"enchantment", MagicSchool::ENCHANTMENT},
    {"evocation", MagicSchool::EVOCATION},   {"illusion", MagicSchool::ILLUSION},
    {"necromancy", MagicSchool::NECROMANCY}, {"transmutation", MagicSchool::TRANSMUTATION},
};

/**
 * @brief An enum for the possible spell levels
 */
enum SpellLevel {
    CANTRIP = 0,
    LEVEL1 = 1,
    LEVEL2 = 2,
    LEVEL3 = 3,
    LEVEL4 = 4,
    LEVEL5 = 5,
    LEVEL6 = 6,
    LEVEL7 = 7,
    LEVEL8 = 8,
    LEVEL9 = 9,
};

/**
 * @brief A struct representing the type of a spell
 */
struct SpellType {
    // the magic school this spell belongs to
    MagicSchool magic_school;
    // the level of the spell
    SpellLevel level;
    // "true" if this spell can be cast as a ritual, "false" otherwise
    bool is_ritual = false;
    /**
     * @brief Returns the level of the spell as a numeric value between 0 and 9
     * @return the level of the spell between 1 and 9, and 0 for cantrips
     */
    int levelAsNumber() const;
};

inline int SpellType::levelAsNumber() const { return static_cast<int>(level); }

/**
 * @brief A struct representing the components required to cast a spell
 */
struct SpellComponents {
    // "true" if verbal components are required to cast the spell, "false" otherwise
    bool verbal = false;
    // "true" if somatic components i.e. hand movements are required to cast the spell, "false" otherwise
    bool somatic = false;
    // "true" if material components are required to cast the spell, "false" otherwise
    bool material = false;
    // a description of the material components required to cast the spell
    std::string materials_needed = "";
};

/**
 * @brief A class representing a spell
 */
class Spell {
public:
    /**
     * @brief Constructs a spell
     * @param name the name of the spell
     * @param type the type of the spell
     * @param casting_time a description of how long the spell takes to cast
     * @param range a description of the range of the spell
     * @param components the components required to cast the spell
     * @param duration a description of how long the effect of the spell lasts
     * @param description a full description of the spell
     * @param classes the names of classes (and subclasses) that can cast this spell
     */
    Spell(
        const std::string& name, const SpellType& type, const std::string& casting_time, const std::string& range,
        const SpellComponents& components, const std::string& duration, const std::string& description,
        const std::unordered_set<std::string>& classes
    ) noexcept;

    // the name of the spell
    const std::string name;
    // a description of how long the spell takes to cast
    const std::string casting_time;
    // a description of the range of the spell
    const std::string range;
    // a description of how long the effect of the spell lasts
    const std::string duration;
    // a full description of the spell
    const std::string description;
    // the names of classes (and subclasses) that can cast this spell
    const std::unordered_set<std::string> classes;
    // the type of the spell
    const SpellType type;
    // the components required to cast the spell
    const SpellComponents components;
};

inline Spell::Spell(
    const std::string& name, const SpellType& type, const std::string& casting_time, const std::string& range,
    const SpellComponents& components, const std::string& duration, const std::string& description,
    const std::unordered_set<std::string>& classes
) noexcept
    : name(name), casting_time(casting_time), range(range), duration(duration), description(description),
      classes(classes), type(type), components(components) {}

} // namespace dnd

#endif // SPELL_HPP_
