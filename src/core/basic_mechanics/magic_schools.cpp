#include <dnd_config.hpp>

#include "magic_schools.hpp"

#include <array>
#include <optional>
#include <string>
#include <string_view>

#include <fmt/format.h>
#include <tl/expected.hpp>

#include <core/errors/runtime_error.hpp>

namespace dnd {

inline constexpr std::array<std::pair<std::string_view, MagicSchool>, 8> magic_schools = {
    std::pair("abjuration", MagicSchool::ABJURATION), std::pair("conjuration", MagicSchool::CONJURATION),
    std::pair("divination", MagicSchool::DIVINATION), std::pair("enchantment", MagicSchool::ENCHANTMENT),
    std::pair("evocation", MagicSchool::EVOCATION),   std::pair("illusion", MagicSchool::ILLUSION),
    std::pair("necromancy", MagicSchool::NECROMANCY), std::pair("transmutation", MagicSchool::TRANSMUTATION),
};

std::optional<MagicSchool> magic_school_from_char(char c) {
    switch (c) {
        case 'A':
            return MagicSchool::ABJURATION;
        case 'C':
            return MagicSchool::CONJURATION;
        case 'D':
            return MagicSchool::DIVINATION;
        case 'E':
            return MagicSchool::ENCHANTMENT;
        case 'I':
            return MagicSchool::ILLUSION;
        case 'N':
            return MagicSchool::NECROMANCY;
        case 'T':
            return MagicSchool::TRANSMUTATION;
        case 'V':
            return MagicSchool::EVOCATION;
        default:
            return std::nullopt;
    }
}

bool is_magic_school(std::string_view magic_school_name) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school_name == school_name) {
            return true;
        }
    }
    return false;
}

bool is_magic_school(const std::string& magic_school_name) {
    return is_magic_school(std::string_view(magic_school_name));
}

tl::expected<std::string_view, RuntimeError> magic_school_name(MagicSchool magic_school) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school == school_val) {
            return std::string_view(school_name);
        }
    }
    return tl::unexpected(RuntimeError(RuntimeError::Code::UNREACHABLE, "Unknown magic school enum value."));
}

tl::expected<MagicSchool, RuntimeError> magic_school_from_string_view(std::string_view magic_school_name) {
    for (const auto& [school_name, school_val] : magic_schools) {
        if (magic_school_name == school_name) {
            return school_val;
        }
    }
    return tl::unexpected(RuntimeError(
        RuntimeError::Code::INVALID_ARGUMENT, fmt::format("The magic school \"{}\" does not exist.", magic_school_name)
    ));
}

tl::expected<MagicSchool, RuntimeError> magic_school_from_string(const std::string& magic_school_name) {
    return magic_school_from_string_view(std::string_view(magic_school_name));
}

} // namespace dnd
