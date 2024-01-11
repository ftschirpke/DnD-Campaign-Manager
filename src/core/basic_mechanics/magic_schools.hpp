#ifndef MAGIC_SCHOOLS_HPP_
#define MAGIC_SCHOOLS_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>

#include <tl/expected.hpp>

#include <core/errors/runtime_error.hpp>

namespace dnd {

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

bool is_magic_school(std::string_view magic_school_name);
bool is_magic_school(const std::string& magic_school_name);

tl::expected<std::string_view, RuntimeError> magic_school_name(MagicSchool magic_school);

tl::expected<MagicSchool, RuntimeError> magic_school_from_string_view(std::string_view magic_school_name);
tl::expected<MagicSchool, RuntimeError> magic_school_from_string(const std::string& magic_school_name);

} // namespace dnd

#endif // MAGIC_SCHOOLS_HPP_
