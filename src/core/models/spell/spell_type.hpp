#ifndef SPELL_TYPE_HPP_
#define SPELL_TYPE_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>

#include <core/utils/data_result.hpp>
#include <core/validation/spell/spell_type_data.hpp>

namespace dnd {

enum class SpellLevel {
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

enum class MagicSchool {
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

bool is_magic_school(const std::string& magic_school_name);
bool is_magic_school(std::string_view magic_school_name);
/**
 * @throws std::out_of_range if no magic school with that value exists
 */
std::string_view magic_school_name(MagicSchool magic_school);
/**
 * @throws std::out_of_range if no magic school with that name exists
 */
MagicSchool magic_school_from_name(const std::string& magic_school_name);
/**
 * @throws std::out_of_range if no magic school with that name exists
 */
MagicSchool magic_school_from_name(std::string_view magic_school_name);

class SpellType {
public:
    using Data = SpellTypeData;

    static CreateResult<SpellType> create(Data&& type_data);

    SpellType(SpellLevel spell_level, MagicSchool magic_school, bool is_ritual) noexcept;

    SpellLevel get_spell_level() const noexcept;
    MagicSchool get_magic_school() const noexcept;
    bool is_ritual() const noexcept;

    /**
     * @brief Returns the spell level as an integer (0 for cantrips, 1 for level 1 spells, etc.)
     * @return the spell level as an integer
     */
    int get_spell_level_int() const;
    std::string_view get_magic_school_name() const;

    /**
     * @brief Creates a string representation of the spell type
     * examples: "Cantrip - School of Illusion" or "3rd-level spell - School of Necromancy (ritual)"
     * @return a string representation of the spell type
     */
    std::string str() const;
    /**
     * @brief Creates a short string representation of the spell type
     * examples: "illusion cantrip" or "3rd level necromancy (ritual)"
     * @return a short string representation of the spell type
     */
    std::string short_str() const;
private:
    SpellLevel spell_level;
    MagicSchool magic_school;
    bool ritual;
};

} // namespace dnd

#endif // SPELL_TYPE_HPP_
