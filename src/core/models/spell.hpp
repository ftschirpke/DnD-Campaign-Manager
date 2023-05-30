#ifndef SPELL_HPP_
#define SPELL_HPP_

#include "dnd_config.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "models/content_piece.hpp"

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

// the magic schools paired with their names as c-style strings
inline constexpr std::array<std::pair<const char*, MagicSchool>, 8> magic_schools = {
    std::pair("abjuration", MagicSchool::ABJURATION), std::pair("conjuration", MagicSchool::CONJURATION),
    std::pair("divination", MagicSchool::DIVINATION), std::pair("enchantment", MagicSchool::ENCHANTMENT),
    std::pair("evocation", MagicSchool::EVOCATION),   std::pair("illusion", MagicSchool::ILLUSION),
    std::pair("necromancy", MagicSchool::NECROMANCY), std::pair("transmutation", MagicSchool::TRANSMUTATION),
};

constexpr const char* magicSchoolName(MagicSchool magic_school) {
    for (const auto& [ms_name, ms_val] : magic_schools) {
        if (magic_school == ms_val) {
            return ms_name;
        }
    }
    throw std::out_of_range("The magic school does not exist.");
}

/**
 * @brief Determines the magic school given its name
 * @param magic_school_name the name of the magic school
 * @return the name of the magic school with the given name
 * @throws std::out_of_range if no magic school with that name exists
 */
constexpr MagicSchool magicSchoolFromName(const std::string& magic_school_name) {
    for (const auto& [ms_name, ms_val] : magic_schools) {
        if (magic_school_name == ms_name) {
            return ms_val;
        }
    }
    throw std::out_of_range("The magic school \"" + magic_school_name + "\" does not exist.");
}

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
    /**
     * @brief Create the string representation of the SpellType object
     * @return the string representation
     */
    std::string str() const;
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
    /**
     * @brief Create a short string representation of the SpellComponents object
     * (without the description of the required materials)
     * @return the short string representation
     */
    std::string shortStr() const;
    /**
     * @brief Create the string representation of the SpellComponents object
     * @return the string representation
     */
    std::string str() const;
};

/**
 * @brief A class representing a spell
 */
class Spell : public ContentPiece {
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
        const std::string& name, const std::filesystem::path& source_file_path, const SpellType& type,
        const std::string& casting_time, const std::string& range, const SpellComponents& components,
        const std::string& duration, const std::string& description, const std::unordered_set<std::string>& classes
    ) noexcept;

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
    const std::string& name, const std::filesystem::path& source_file_path, const SpellType& type,
    const std::string& casting_time, const std::string& range, const SpellComponents& components,
    const std::string& duration, const std::string& description, const std::unordered_set<std::string>& classes
) noexcept
    : ContentPiece(name, source_file_path), casting_time(casting_time), range(range), duration(duration),
      description(description), classes(classes), type(type), components(components) {}

} // namespace dnd

#endif // SPELL_HPP_
