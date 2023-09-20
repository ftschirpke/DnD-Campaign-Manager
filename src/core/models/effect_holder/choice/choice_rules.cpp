#include <dnd_config.hpp>

#include "choice_rules.hpp"

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

static constexpr std::array<std::pair<std::string_view, dnd::ChoiceType>, 22> valid_attribute_names = {
    std::pair("effects", dnd::ChoiceType::EFFECT),
    std::pair("cantrips_free", dnd::ChoiceType::SPELL),
    std::pair("spells_at_will", dnd::ChoiceType::SPELL),
    std::pair("spells_innate", dnd::ChoiceType::SPELL),
    std::pair("spells_free_once_a_day", dnd::ChoiceType::SPELL),
    std::pair("spells_known", dnd::ChoiceType::SPELL),
    std::pair("spells_known_included", dnd::ChoiceType::SPELL),
    std::pair("spells_added_to_spell_list", dnd::ChoiceType::SPELL),
    std::pair("spells_always_prepared", dnd::ChoiceType::SPELL),
    std::pair("saving_throw_proficiencies", dnd::ChoiceType::ABILITY),
    std::pair("skill_proficiencies", dnd::ChoiceType::SKILL),
    std::pair("armor_proficiencies", dnd::ChoiceType::STRING),
    std::pair("weapon_proficiencies", dnd::ChoiceType::STRING),
    std::pair("tool_proficiencies", dnd::ChoiceType::STRING),
    std::pair("languages", dnd::ChoiceType::STRING),
    std::pair("senses", dnd::ChoiceType::STRING),
    std::pair("damage_resistances", dnd::ChoiceType::STRING),
    std::pair("damage_immunities", dnd::ChoiceType::STRING),
    std::pair("damage_vulnerabilities", dnd::ChoiceType::STRING),
    std::pair("condition_immunities", dnd::ChoiceType::STRING),
};

bool dnd::is_valid_choice_attribute_name(const std::string& attribute_name) noexcept {
    for (const auto& valid_attribute_name : valid_attribute_names) {
        if (valid_attribute_name.first == attribute_name) {
            return true;
        }
    }
    return false;
}

dnd::ChoiceType dnd::choice_type_for_attribute_name(const std::string& attribute_name) {
    for (const auto& valid_attribute_name : valid_attribute_names) {
        if (valid_attribute_name.first == attribute_name) {
            return valid_attribute_name.second;
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

bool dnd::attribute_name_implies_group(const std::string& attribute_name) noexcept {
    for (const auto& group_name : valid_group_names) {
        if (group_name.first == attribute_name) {
            return true;
        }
    }
    return false;
}

std::string dnd::group_name_for_attribute_name(const std::string& attribute_name) {
    for (const auto& group_name : valid_group_names) {
        if (group_name.first == attribute_name) {
            return std::string(group_name.second);
        }
    }
    throw std::invalid_argument("Attribute name '" + attribute_name + "' does not imply a group of strings");
}
