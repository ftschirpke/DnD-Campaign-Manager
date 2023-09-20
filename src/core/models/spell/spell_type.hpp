#ifndef SPELL_TYPE_HPP_
#define SPELL_TYPE_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>

#include <core/validation/spell/spell_type_data.hpp>

namespace dnd {

/**
 * @brief An enum for the possible spell levels
 */
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

/**
 * @brief An enum for the types of magic or so called magic schools
 */
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

/**
 * @brief Returns whether or not the given string is a valid magic school name
 * @param magic_school_name the name of the magic school
 * @return "true" if the given string is a valid magic school name, "false" otherwise
 */
bool is_magic_school(const std::string& magic_school_name);
/**
 * @brief Returns whether or not the given string is a valid magic school name
 * @param magic_school_name the name of the magic school
 * @return "true" if the given string is a valid magic school name, "false" otherwise
 */
bool is_magic_school(std::string_view magic_school_name);
/**
 * @brief Returns the name of the given magic school
 * @param magic_school the magic school value
 * @return the name of the given magic school
 * @throws std::out_of_range if no magic school with that value exists
 */
std::string_view magic_school_name(MagicSchool magic_school);
/**
 * @brief Returns the magic school given its name
 * @param magic_school_name the name of the magic school
 * @return the name of the magic school with the given name
 * @throws std::out_of_range if no magic school with that name exists
 */
MagicSchool magic_school_from_name(const std::string& magic_school_name);
/**
 * @brief Returns the magic school given its name
 * @param magic_school_name the name of the magic school
 * @return the name of the magic school with the given name
 * @throws std::out_of_range if no magic school with that name exists
 */
MagicSchool magic_school_from_name(std::string_view magic_school_name);

/**
 * @brief A class representing the type of a spell (its spell level, magic school, and whether or not it can be
 * cast as a ritual)
 */
class SpellType {
public:
    static SpellType create(SpellTypeData&& type_data);

    SpellType(SpellLevel spell_level, MagicSchool magic_school, bool is_ritual) noexcept;

    SpellLevel get_spell_level() const noexcept;
    MagicSchool get_magic_school() const noexcept;
    bool is_ritual() const noexcept;

    /**
     * @brief Returns the spell level as an integer (0 for cantrips, 1 for level 1 spells, etc.)
     * @return the spell level as an integer
     */
    int get_spell_level_int() const;
    /**
     * @brief Returns the name of the magic school
     * @return the name of the magic school
     */
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
