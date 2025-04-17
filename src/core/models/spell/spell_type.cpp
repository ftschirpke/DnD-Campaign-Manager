#include <dnd_config.hpp>

#include "spell_type.hpp"

#include <cassert>
#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>
#include <tl/expected.hpp>

#include <core/basic_mechanics/magic_schools.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/spell/spell_type_validation.hpp>

namespace dnd {

CreateResult<SpellType> SpellType::create(Data&& data) {
    Errors errors = validate_spell_type(data);
    if (!errors.ok()) {
        return InvalidCreate<SpellType>(std::move(data), std::move(errors));
    }

    SpellLevel level = static_cast<SpellLevel>(data.level);
    MagicSchool magic_school = magic_school_from_char(data.magic_school_char).value();

    return ValidCreate(SpellType(level, magic_school, data.ritual));
}

SpellType::SpellType(SpellLevel spell_level, MagicSchool magic_school, bool is_ritual)
    : spell_level(spell_level), magic_school(magic_school), ritual(is_ritual) {}

SpellLevel SpellType::get_spell_level() const { return spell_level; }

MagicSchool SpellType::get_magic_school() const { return magic_school; }

bool SpellType::is_ritual() const { return ritual; }

int SpellType::get_spell_level_as_int() const { return static_cast<int>(spell_level); }

std::string_view SpellType::get_magic_school_name() const {
    tl::expected<std::string_view, RuntimeError> magic_school_name_result = magic_school_name(magic_school);
    assert(magic_school_name_result.has_value());
    return magic_school_name_result.value();
}

std::string SpellType::str() const {
    std::string capitalized_school_name = std::string(get_magic_school_name());
    capitalized_school_name[0] = char_to_uppercase(capitalized_school_name[0]);
    switch (spell_level) {
        case SpellLevel::CANTRIP:
            return fmt::format("Cantrip - School of {}", capitalized_school_name);
        case SpellLevel::LEVEL1:
            return fmt::format("1st-level spell - School of {}", capitalized_school_name);
        case SpellLevel::LEVEL2:
            return fmt::format("2nd-level spell - School of {}", capitalized_school_name);
        case SpellLevel::LEVEL3:
            return fmt::format("3rd-level spell - School of {}", capitalized_school_name);
        case SpellLevel::LEVEL4:
        case SpellLevel::LEVEL5:
        case SpellLevel::LEVEL6:
        case SpellLevel::LEVEL7:
        case SpellLevel::LEVEL8:
        case SpellLevel::LEVEL9:
            return fmt::format("{}th-level spell - School of {}", get_spell_level_as_int(), capitalized_school_name);
    }
    assert(false);
    return "";
}

std::string SpellType::short_str() const {
    switch (spell_level) {
        case SpellLevel::CANTRIP:
            return fmt::format("{} cantrip", get_magic_school_name());
        case SpellLevel::LEVEL1:
            return fmt::format("1st-level {}", get_magic_school_name());
        case SpellLevel::LEVEL2:
            return fmt::format("2nd-level {}", get_magic_school_name());
        case SpellLevel::LEVEL3:
            return fmt::format("3rd-level {}", get_magic_school_name());
        case SpellLevel::LEVEL4:
        case SpellLevel::LEVEL5:
        case SpellLevel::LEVEL6:
        case SpellLevel::LEVEL7:
        case SpellLevel::LEVEL8:
        case SpellLevel::LEVEL9:
            return fmt::format("{}th-level {}", get_spell_level_as_int(), get_magic_school_name());
    }
    assert(false);
    return "";
}

} // namespace dnd
