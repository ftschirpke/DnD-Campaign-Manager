#ifndef ABILITIES_HPP_
#define ABILITIES_HPP_

#include <dnd_config.hpp>

#include <array>
#include <optional>
#include <string>
#include <string_view>

namespace dnd {

enum class Ability {
    STRENGTH,
    DEXTERITY,
    CONSTITUTION,
    INTELLIGENCE,
    WISDOM,
    CHARISMA,
};

inline constexpr std::array<Ability, 6> abilities_inorder = {
    Ability::STRENGTH,     Ability::DEXTERITY, Ability::CONSTITUTION,
    Ability::INTELLIGENCE, Ability::WISDOM,    Ability::CHARISMA,
};

inline constexpr std::array<const char*, 6> ability_cstrings_inorder = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};

std::optional<Ability> ability_from_string(const std::string& ability_str);
std::string ability_name(Ability ability);

bool is_ability(std::string_view attribute_name);
bool is_ability(const std::string& attribute_name);

} // namespace dnd

#endif // ABILITIES_HPP_
