#ifndef SPELL_HPP_
#define SPELL_HPP_

#include <string>
#include <unordered_map>

namespace dnd {

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

const std::unordered_map<std::string, MagicSchool> magic_schools = {
    {"abjuration", MagicSchool::ABJURATION}, {"conjuration", MagicSchool::CONJURATION},
    {"divination", MagicSchool::DIVINATION}, {"enchantment", MagicSchool::ENCHANTMENT},
    {"evocation", MagicSchool::EVOCATION},   {"illusion", MagicSchool::ILLUSION},
    {"necromancy", MagicSchool::NECROMANCY}, {"transmutation", MagicSchool::TRANSMUTATION},
};

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

struct SpellType {
    MagicSchool magic_school;
    SpellLevel level;
    bool is_ritual = false;
};

struct SpellComponents {
    bool verbal = false;
    bool somatic = false;
    bool material = false;
    std::string materials_needed = "";
};

class Spell {
public:
    const std::string name, casting_time, range, duration, description;
    const SpellType type;
    const SpellComponents components;
    Spell() = delete;
    Spell(
        const std::string& name, const SpellType& type, const std::string& casting_time, const std::string& range,
        const SpellComponents& components, const std::string& duration, const std::string& description
    );
};

inline Spell::Spell(
    const std::string& name, const SpellType& type, const std::string& casting_time, const std::string& range,
    const SpellComponents& components, const std::string& duration, const std::string& description
)
    : name(name), type(type), casting_time(casting_time), range(range), components(components), duration(duration),
      description(description) {}

} // namespace dnd

#endif // SPELL_HPP_
