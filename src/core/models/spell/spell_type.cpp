#include <dnd_config.hpp>

#include "spell_type.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>

#include <core/utils/char_manipulation.hpp>

inline constexpr std::array<std::pair<std::string_view, MagicSchool>, 8> magic_schools = {
    std::pair("abjuration", MagicSchool::ABJURATION), std::pair("conjuration", MagicSchool::CONJURATION),
    std::pair("divination", MagicSchool::DIVINATION), std::pair("enchantment", MagicSchool::ENCHANTMENT),
    std::pair("evocation", MagicSchool::EVOCATION),   std::pair("illusion", MagicSchool::ILLUSION),
    std::pair("necromancy", MagicSchool::NECROMANCY), std::pair("transmutation", MagicSchool::TRANSMUTATION),
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
    throw std::out_of_range("The magic school \"" + magic_school_name + "\" does not exist.");
}

dnd::MagicSchool dnd::magic_school_from_name(std::string_view magic_school_name) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school_name == school_name) {
            return school_val;
        }
    }
    throw std::out_of_range("The magic school \"" + magic_school_name + "\" does not exist.");
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
