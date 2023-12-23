#include <algorithm>
#include <dnd_config.hpp>

#include "core/basic_mechanics/skills.hpp"
#include "stats.hpp"

#include <cassert>
#include <string>
#include <unordered_map>

dnd::Stats dnd::Stats::create_default() noexcept {
    Stats stats;
    return stats;
}

dnd::Stats::Stats() noexcept {
    mutable_values["STR"] = 10;
    mutable_values["DEX"] = 10;
    mutable_values["CON"] = 10;
    mutable_values["INT"] = 10;
    mutable_values["WIS"] = 10;
    mutable_values["CHA"] = 10;
}

bool dnd::Stats::is_complete() const noexcept {
    assert(mutable_values.contains("STR"));
    assert(mutable_values.contains("DEX"));
    assert(mutable_values.contains("CON"));
    assert(mutable_values.contains("INT"));
    assert(mutable_values.contains("WIS"));
    assert(mutable_values.contains("CHA"));
    bool has_ability_modifiers = mutable_values.contains("STR_MOD") && mutable_values.contains("DEX_MOD")
                                 && mutable_values.contains("CON_MOD") && mutable_values.contains("INT_MOD")
                                 && mutable_values.contains("WIS_MOD") && mutable_values.contains("CHA_MOD");
    auto all_skills = get_all_skills();
    bool has_skill_modifiers = std::all_of(all_skills.begin(), all_skills.end(), [this](const std::string& skill) {
        return mutable_values.contains(skill);
    });
    return has_ability_modifiers && has_skill_modifiers;
}
