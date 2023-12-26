#include <dnd_config.hpp>

#include "choice_rules.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace dnd {

static constexpr std::array<std::pair<std::string_view, ChoiceType>, 22> valid_attribute_names = {
    std::pair("stat_changes", ChoiceType::STAT_CHANGE),
    std::pair("cantrips_free", ChoiceType::SPELL),
    std::pair("spells_at_will", ChoiceType::SPELL),
    std::pair("spells_innate", ChoiceType::SPELL),
    std::pair("spells_free_once_a_day", ChoiceType::SPELL),
    std::pair("spells_known", ChoiceType::SPELL),
    std::pair("spells_known_included", ChoiceType::SPELL),
    std::pair("spells_added_to_spell_list", ChoiceType::SPELL),
    std::pair("spells_always_prepared", ChoiceType::SPELL),
    std::pair("saving_throw_proficiencies", ChoiceType::ABILITY),
    std::pair("skill_proficiencies", ChoiceType::SKILL),
    std::pair("armor_proficiencies", ChoiceType::STRING),
    std::pair("weapon_proficiencies", ChoiceType::STRING),
    std::pair("tool_proficiencies", ChoiceType::STRING),
    std::pair("languages", ChoiceType::STRING),
    std::pair("senses", ChoiceType::STRING),
    std::pair("damage_resistances", ChoiceType::STRING),
    std::pair("damage_immunities", ChoiceType::STRING),
    std::pair("damage_vulnerabilities", ChoiceType::STRING),
    std::pair("condition_immunities", ChoiceType::STRING),
};

bool is_valid_choice_attribute_name(const std::string& attribute_name) noexcept {
    for (const auto& [valid_attribute_name, _] : valid_attribute_names) {
        if (valid_attribute_name == attribute_name) {
            return true;
        }
    }
    return false;
}

ChoiceType choice_type_for_attribute_name(const std::string& attribute_name) {
    for (const auto& [valid_attribute_name, choice_type] : valid_attribute_names) {
        if (valid_attribute_name == attribute_name) {
            return choice_type;
        }
    }
    throw std::invalid_argument("Invalid choice attribute name: " + attribute_name);
}

static constexpr std::array<std::pair<std::string_view, std::string_view>, 9> valid_group_names = {
    std::pair("armor_proficiencies", "armor"),
    std::pair("weapon_proficiencies", "weapons"),
    std::pair("tool_proficiencies", "tools"),
    std::pair("languages", "languages"),
    std::pair("senses", "senses"),
    std::pair("damage_resistances", "damage types"),
    std::pair("damage_immunities", "damage types"),
    std::pair("damage_vulnerabilities", "damage types"),
    std::pair("condition_immunities", "conditions"),
};

bool attribute_name_implies_group(const std::string& attribute_name) noexcept {
    for (const auto& [group_name, _] : valid_group_names) {
        if (group_name == attribute_name) {
            return true;
        }
    }
    return false;
}

std::string group_name_for_attribute_name(const std::string& attribute_name) {
    for (const auto& [proficiency_name, group_name] : valid_group_names) {
        if (proficiency_name == attribute_name) {
            return std::string(group_name);
        }
    }
    throw std::invalid_argument("Attribute name '" + attribute_name + "' does not imply a group of strings");
}

} // namespace dnd
