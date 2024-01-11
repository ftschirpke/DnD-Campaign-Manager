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
    bool is_ritual;
    SpellLevel level;
    MagicSchool magic_school;

    size_t ritual_idx = data.str.find(" (ritual)");
    is_ritual = ritual_idx != std::string::npos;
    std::string magic_school_str;
    size_t cantrip_idx = data.str.find(" cantrip");
    if (cantrip_idx != std::string::npos) {
        level = SpellLevel::CANTRIP;
        magic_school_str = data.str.substr(0, cantrip_idx);
    } else {
        assert(std::isdigit(static_cast<unsigned char>(data.str[0])));
        level = SpellLevel(data.str[0] - '0');
        size_t i = data.str.find("level ") + 6;
        if (is_ritual) {
            magic_school_str = data.str.substr(i, ritual_idx - i);
        } else {
            magic_school_str = data.str.substr(i, data.str.size() - i);
        }
    }
    string_lowercase_inplace(magic_school_str);
    tl::expected<MagicSchool, RuntimeError> magic_school_result = magic_school_from_string(magic_school_str);
    if (!magic_school_result.has_value()) {
        Errors errors(magic_school_result.error());
        return InvalidCreate<SpellType>(std::move(data), std::move(errors));
    }
    magic_school = magic_school_result.value();
    return ValidCreate(SpellType(level, magic_school, is_ritual));
}

SpellType::SpellType(SpellLevel spell_level, MagicSchool magic_school, bool is_ritual) noexcept
    : spell_level(spell_level), magic_school(magic_school), ritual(is_ritual) {}

SpellLevel SpellType::get_spell_level() const noexcept { return spell_level; }

MagicSchool SpellType::get_magic_school() const noexcept { return magic_school; }

bool SpellType::is_ritual() const noexcept { return ritual; }

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
        default:
            return fmt::format("{}th-level spell - School of {}", get_spell_level_as_int(), capitalized_school_name);
    }
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
        default:
            return fmt::format("{}th-level {}", get_spell_level_as_int(), get_magic_school_name());
    }
}

} // namespace dnd
