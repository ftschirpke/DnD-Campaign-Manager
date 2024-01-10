#ifndef SPELL_TYPE_HPP_
#define SPELL_TYPE_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>
#include <string_view>

#include <core/basic_mechanics/magic_schools.hpp>
#include <core/utils/data_result.hpp>

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

class SpellType {
public:
    class Data;

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

class SpellType::Data {
public:
    std::strong_ordering operator<=>(const Data&) const noexcept = default;

    std::string str;
};

} // namespace dnd

#endif // SPELL_TYPE_HPP_
