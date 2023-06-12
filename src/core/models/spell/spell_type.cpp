#include <dnd_config.hpp>

#include "spell_type.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/spell/spell_type_data.hpp>

inline constexpr std::array<std::pair<std::string_view, dnd::MagicSchool>, 8> magic_schools = {
    std::pair("abjuration", dnd::MagicSchool::ABJURATION), std::pair("conjuration", dnd::MagicSchool::CONJURATION),
    std::pair("divination", dnd::MagicSchool::DIVINATION), std::pair("enchantment", dnd::MagicSchool::ENCHANTMENT),
    std::pair("evocation", dnd::MagicSchool::EVOCATION),   std::pair("illusion", dnd::MagicSchool::ILLUSION),
    std::pair("necromancy", dnd::MagicSchool::NECROMANCY), std::pair("transmutation", dnd::MagicSchool::TRANSMUTATION),
};

bool dnd::is_magic_school(const std::string& magic_school_name) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school_name == school_name) {
            return true;
        }
    }
    return false;
}

bool dnd::is_magic_school(std::string_view magic_school_name) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school_name == school_name) {
            return true;
        }
    }
    return false;
}

std::string_view dnd::magic_school_name(MagicSchool magic_school) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school == school_val) {
            return school_name;
        }
    }
    throw std::out_of_range("The magic school does not exist.");
}

dnd::MagicSchool dnd::magic_school_from_name(const std::string& magic_school_name) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school_name == school_name) {
            return school_val;
        }
    }
    throw std::out_of_range(fmt::format("The magic school \"{}\" does not exist.", magic_school_name));
}

dnd::MagicSchool dnd::magic_school_from_name(std::string_view magic_school_name) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school_name == school_name) {
            return school_val;
        }
    }
    throw std::out_of_range(fmt::format("The magic school \"{}\" does not exist.", magic_school_name));
}

dnd::SpellType dnd::SpellType::create(dnd::SpellTypeData&& type_data) {
    if (!type_data.validate().ok()) {
        throw invalid_data("Cannot create SpellType from invalid data.");
    }
    bool is_ritual;
    SpellLevel level;
    MagicSchool magic_school;

    size_t ritual_idx = type_data.str.find(" (ritual)");
    is_ritual = ritual_idx != std::string::npos;
    std::string magic_school_str;
    size_t cantrip_idx = type_data.str.find(" cantrip");
    if (cantrip_idx != std::string::npos) {
        level = SpellLevel::CANTRIP;
        magic_school_str = type_data.str.substr(0, cantrip_idx);
    } else {
        assert(std::isdigit(static_cast<unsigned char>(type_data.str[0])));
        level = SpellLevel(type_data.str[0] - '0');
        size_t i = type_data.str.find("level ") + 6;
        if (is_ritual) {
            magic_school_str = type_data.str.substr(i, ritual_idx - i);
        } else {
            magic_school_str = type_data.str.substr(i, type_data.str.size() - i);
        }
    }
    string_to_lowercase(magic_school_str);
    magic_school = magic_school_from_name(magic_school_str);
    return SpellType(level, magic_school, is_ritual);
}

dnd::SpellType::SpellType(dnd::SpellLevel spell_level, dnd::MagicSchool magic_school, bool is_ritual) noexcept
    : spell_level(spell_level), magic_school(magic_school), is_ritual(is_ritual) {}

dnd::SpellLevel dnd::SpellType::get_spell_level() const noexcept { return spell_level; }

dnd::MagicSchool dnd::SpellType::get_magic_school() const noexcept { return magic_school; }

bool dnd::SpellType::get_is_ritual() const noexcept { return is_ritual; }

int dnd::SpellType::get_spell_level_int() const { return static_cast<int>(spell_level); }

std::string_view dnd::SpellType::get_magic_school_name() const { return magic_school_name(magic_school); }

std::string dnd::SpellType::str() const {
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
            return fmt::format("{}th-level spell - School of {}", get_spell_level_int(), capitalized_school_name);
    }
}

std::string dnd::SpellType::short_str() const {
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
            return fmt::format("{}th-level {}", get_spell_level_int(), get_magic_school_name());
    }
}