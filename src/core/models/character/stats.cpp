#include <dnd_config.hpp>

#include "stats.hpp"

#include <algorithm>
#include <cassert>
#include <string>
#include <unordered_map>

#include <fmt/format.h>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>

namespace dnd {

static int calculate_modifier(int score) noexcept { return score / 2 - 5; }

Stats Stats::create_default() noexcept {
    Stats stats;
    return stats;
}

Stats Stats::create_from_base_scores_and_stat_changes(
    const AbilityScores& base_ability_scores, std::vector<CRef<StatChange>> stat_changes
) noexcept {
    Stats stats;
    stats.mutable_values["STR"] = base_ability_scores.get_strength();
    stats.mutable_values["DEX"] = base_ability_scores.get_dexterity();
    stats.mutable_values["CON"] = base_ability_scores.get_constitution();
    stats.mutable_values["INT"] = base_ability_scores.get_intelligence();
    stats.mutable_values["WIS"] = base_ability_scores.get_wisdom();
    stats.mutable_values["CHA"] = base_ability_scores.get_charisma();

    // TODO: Apply all stat changes for the ability scores

    stats.calculate_ability_modifiers();
    stats.calculate_skill_modifiers();

    // TODO: Apply all other stat changes

    return stats;
}

Stats::Stats() noexcept {
    mutable_values["STR"] = 10;
    mutable_values["DEX"] = 10;
    mutable_values["CON"] = 10;
    mutable_values["INT"] = 10;
    mutable_values["WIS"] = 10;
    mutable_values["CHA"] = 10;
}

bool Stats::is_complete() const noexcept {
    assert(mutable_values.contains("STR"));
    assert(mutable_values.contains("DEX"));
    assert(mutable_values.contains("CON"));
    assert(mutable_values.contains("INT"));
    assert(mutable_values.contains("WIS"));
    assert(mutable_values.contains("CHA"));
    bool has_ability_modifiers = mutable_values.contains("STR_MOD") && mutable_values.contains("DEX_MOD")
                                 && mutable_values.contains("CON_MOD") && mutable_values.contains("INT_MOD")
                                 && mutable_values.contains("WIS_MOD") && mutable_values.contains("CHA_MOD");
    std::vector<std::string> all_skills = get_all_skills();
    bool has_skill_modifiers = std::all_of(all_skills.begin(), all_skills.end(), [this](const std::string& skill) {
        return mutable_values.contains(skill);
    });
    return has_ability_modifiers && has_skill_modifiers;
}

void Stats::calculate_ability_modifiers() noexcept {
    assert(mutable_values.contains("STR"));
    implied_values["STR_MOD"] = (mutable_values["STR"] - 10) / 2;
    assert(mutable_values.contains("DEX"));
    implied_values["DEX_MOD"] = (mutable_values["DEX"] - 10) / 2;
    assert(mutable_values.contains("CON"));
    implied_values["CON_MOD"] = (mutable_values["CON"] - 10) / 2;
    assert(mutable_values.contains("INT"));
    implied_values["INT_MOD"] = (mutable_values["INT"] - 10) / 2;
    assert(mutable_values.contains("WIS"));
    implied_values["WIS_MOD"] = (mutable_values["WIS"] - 10) / 2;
    assert(mutable_values.contains("CHA"));
    implied_values["CHA_MOD"] = (mutable_values["CHA"] - 10) / 2;
}

void Stats::calculate_skill_modifiers() noexcept {
    for (const auto& [skill, ability] : get_abilities_for_all_skills()) {
        const std::string ability_modifier = ability + "_MOD";
        assert(implied_values.contains(ability_modifier));
        mutable_values[skill] = implied_values[ability_modifier];
    }
}

} // namespace dnd
