#include <dnd_config.hpp>

#include "skills.hpp"

#include <array>
#include <string_view>

#include <core/basic_mechanics/abilities.hpp>

namespace dnd {

static constexpr std::array<const SkillInfo, 18> skill_infos = {
    SkillInfo{Skill::ACROBATICS, Ability::DEXTERITY, "ACROBATICS", "Acrobatics", "acrobatics"},
    SkillInfo{Skill::ANIMAL_HANDLING, Ability::WISDOM, "ANIMAL_HANDLING", "Animal Handling", "animal handling"},
    SkillInfo{Skill::ARCANA, Ability::INTELLIGENCE, "ARCANA", "Arcana", "arcana"},
    SkillInfo{Skill::ATHLETICS, Ability::STRENGTH, "ATHLETICS", "Athletics", "athletics"},
    SkillInfo{Skill::DECEPTION, Ability::CHARISMA, "DECEPTION", "Deception", "deception"},
    SkillInfo{Skill::HISTORY, Ability::INTELLIGENCE, "HISTORY", "History", "history"},
    SkillInfo{Skill::INSIGHT, Ability::WISDOM, "INSIGHT", "Insight", "insight"},
    SkillInfo{Skill::INTIMIDATION, Ability::CHARISMA, "INTIMIDATION", "Intimidation", "intimidation"},
    SkillInfo{Skill::INVESTIGATION, Ability::INTELLIGENCE, "INVESTIGATION", "Investigation", "investigation"},
    SkillInfo{Skill::MEDICINE, Ability::WISDOM, "MEDICINE", "Medicine", "medicine"},
    SkillInfo{Skill::NATURE, Ability::INTELLIGENCE, "NATURE", "Nature", "nature"},
    SkillInfo{Skill::PERCEPTION, Ability::WISDOM, "PERCEPTION", "Perception", "perception"},
    SkillInfo{Skill::PERFORMANCE, Ability::CHARISMA, "PERFORMANCE", "Performance", "performance"},
    SkillInfo{Skill::PERSUASION, Ability::CHARISMA, "PERSUASION", "Persuasion", "persuasion"},
    SkillInfo{Skill::RELIGION, Ability::INTELLIGENCE, "RELIGION", "Religion", "religion"},
    SkillInfo{Skill::SLEIGHT_OF_HAND, Ability::DEXTERITY, "SLEIGHT_OF_HAND", "Sleight of Hand", "sleight of hand"},
    SkillInfo{Skill::STEALTH, Ability::DEXTERITY, "STEALTH", "Stealth", "stealth"},
    SkillInfo{Skill::SURVIVAL, Ability::WISDOM, "SURVIVAL", "Survival", "survival"},
};

bool is_skill(std::string_view skill) {
    return skill_from_config_name(skill).has_value() || skill_from_display_name(skill).has_value()
           || skill_from_stat_name(skill).has_value();
}

std::optional<SkillInfo> get_skill_info(Skill skill) {
    for (const SkillInfo& skill_info : skill_infos) {
        if (skill_info.skill == skill) {
            return skill_info;
        }
    }
    return std::nullopt;
}

const std::array<const SkillInfo, 18>& get_all_skill_infos() { return skill_infos; }

std::optional<Skill> skill_from_config_name(std::string_view config_name) {
    for (const SkillInfo& skill_info : skill_infos) {
        if (skill_info.config_name == config_name) {
            return skill_info.skill;
        }
    }
    return std::nullopt;
}

std::optional<Skill> skill_from_display_name(std::string_view display_name) {
    for (const SkillInfo& skill_info : skill_infos) {
        if (skill_info.display_name == display_name) {
            return skill_info.skill;
        }
    }
    return std::nullopt;
}

std::optional<Skill> skill_from_stat_name(std::string_view stat_name) {
    for (const SkillInfo& skill_info : skill_infos) {
        if (skill_info.stat_name == stat_name) {
            return skill_info.skill;
        }
    }
    return std::nullopt;
}

} // namespace dnd
