#include <dnd_config.hpp>

#include "skills.hpp"

#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <core/utils/char_manipulation.hpp>

namespace dnd {

static constexpr std::array<std::pair<std::string_view, std::string_view>, 18> skill_abilities = {
    std::pair("ACROBATICS", "DEX"),      std::pair("ANIMAL_HANDLING", "WIS"), std::pair("ARCANA", "INT"),
    std::pair("ATHLETICS", "STR"),       std::pair("DECEPTION", "CHA"),       std::pair("HISTORY", "INT"),
    std::pair("INSIGHT", "WIS"),         std::pair("INTIMIDATION", "CHA"),    std::pair("INVESTIGATION", "INT"),
    std::pair("MEDICINE", "WIS"),        std::pair("NATURE", "INT"),          std::pair("PERCEPTION", "WIS"),
    std::pair("PERFORMANCE", "CHA"),     std::pair("PERSUASION", "CHA"),      std::pair("RELIGION", "INT"),
    std::pair("SLEIGHT_OF_HAND", "DEX"), std::pair("STEALTH", "DEX"),         std::pair("SURVIVAL", "WIS"),
};

bool is_skill(const std::string& skill) noexcept {
    for (const auto& skill_ability_pair : skill_abilities) {
        std::string_view skill_to_check = skill_ability_pair.first;
        if (skill.size() != skill_to_check.size()) {
            continue;
        }
        bool equal = true;
        size_t i = 0;
        while (equal && i < skill.size()) {
            if (skill[i] == ' ') {
                if (skill_to_check[i] != '_' && skill_to_check[i] != ' ') {
                    equal = false;
                }
            } else if (char_to_uppercase(skill[i]) != skill_to_check[i]) {
                equal = false;
            }
            ++i;
        }
        if (equal && i == skill.size()) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> get_all_skills() noexcept {
    std::vector<std::string> skills;
    skills.reserve(skill_abilities.size());
    for (const auto& [skill_name, _] : skill_abilities) {
        std::string& skill_str = skills.emplace_back(skill_name);
        std::transform(skill_str.begin(), skill_str.end(), skill_str.begin(), [](char c) {
            if (c == '_') {
                return ' ';
            }
            return char_to_lowercase(c);
        });
    }
    return skills;
}

std::map<std::string, std::string> get_abilities_for_all_skills() noexcept {
    std::map<std::string, std::string> skill_ability_map;
    for (const auto& skill_ability_pair : skill_abilities) {
        skill_ability_map.emplace(skill_ability_pair);
    }
    return skill_ability_map;
}

} // namespace dnd
