#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include <dnd_config.hpp>

#include <string_view>

#include <core/basic_mechanics/abilities.hpp>

namespace dnd {

enum class Skill {
    ACROBATICS,
    ANIMAL_HANDLING,
    ARCANA,
    ATHLETICS,
    DECEPTION,
    HISTORY,
    INSIGHT,
    INTIMIDATION,
    INVESTIGATION,
    MEDICINE,
    NATURE,
    PERCEPTION,
    PERFORMANCE,
    PERSUASION,
    RELIGION,
    SLEIGHT_OF_HAND,
    STEALTH,
    SURVIVAL,
};

struct SkillInfo {
    Skill skill;
    Ability ability;
    const char* stat_name;
    const char* display_name;
    const char* config_name;
};

bool is_skill(std::string_view skill);

std::optional<SkillInfo> get_skill_info(Skill skill);

const std::array<const SkillInfo, 18>& get_all_skill_infos();

std::optional<Skill> skill_from_config_name(std::string_view display_name);

std::optional<Skill> skill_from_display_name(std::string_view display_name);

std::optional<Skill> skill_from_stat_name(std::string_view stat_name);

} // namespace dnd

#endif // SKILLS_HPP_
