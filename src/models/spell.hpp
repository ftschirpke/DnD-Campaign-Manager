#ifndef SPELL_HPP_
#define SPELL_HPP_

#include <string>

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
    bool is_ritual;
};

class Spell {
public:
    const std::string name;
    const SpellType type;
    Spell(const std::string& name);
};

} // namespace dnd

#endif // SPELL_HPP_
