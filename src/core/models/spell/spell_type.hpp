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

    int get_spell_level_as_int() const;
    std::string_view get_magic_school_name() const;

    // returns string of form: "Cantrip - School of Illusion" or "3rd-level spell - School of Necromancy (ritual)"
    std::string str() const;
    // returns string of form: "illusion cantrip" or "3rd level necromancy (ritual)"
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
