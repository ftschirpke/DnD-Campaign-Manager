#include <dnd_config.hpp>

#include "stats.hpp"

#include <algorithm>
#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>

#include <fmt/format.h>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>

namespace dnd {

static int calculate_modifier(int score) { return score / 2 - 5; }

Stats Stats::create_default() {
    Stats stats;
    return stats;
}

Stats Stats::create_from_base_scores_and_stat_changes(
    const AbilityScores& base_ability_scores, std::vector<CRef<StatChange>> stat_changes
) {
    DND_UNUSED(stat_changes);
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

Stats::Stats()
    : mutable_values({
        {"STR_MOD", 10},
        {"DEX_MOD", 10},
        {"CON_MOD", 10},
        {"INT_MOD", 10},
        {"WIS_MOD", 10},
        {"CHA_MOD", 10},
    }) {}

bool Stats::is_complete() const {
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

std::optional<int> Stats::get(const std::string& name) const {
    auto const_it = constant_values.find(name);
    if (const_it != constant_values.end()) {
        return const_it->second;
    }
    auto it = mutable_values.find(name);
    if (it != mutable_values.end()) {
        return it->second;
    }
    it = implied_values.find(name);
    if (it != implied_values.end()) {
        return it->second;
    }
    return std::nullopt;
}

int Stats::get_or_default(const std::string& name) const { return get_or_else(name, 0); }

int Stats::get_or_else(const std::string& name, int default_value) const {
    std::optional<int> value = get(name);
    if (value.has_value()) {
        return value.value();
    }
    return default_value;
}

std::optional<Ref<int>> Stats::get_mut(const std::string& name) {
    auto it = mutable_values.find(name);
    if (it != mutable_values.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

int& Stats::get_mut_or_default(const std::string& name) {
    std::optional<Ref<int>> value = get_mut(name);
    if (value.has_value()) {
        return value.value().get();
    }
    return mutable_values[name];
}

int& Stats::get_mut_or_else(const std::string& name, int default_value) {
    std::optional<Ref<int>> value = get_mut(name);
    if (value.has_value()) {
        return value.value().get();
    }
    return mutable_values[name] = default_value;
}

void Stats::calculate_ability_modifiers() {
    assert(mutable_values.contains("STR"));
    assert(mutable_values.contains("DEX"));
    assert(mutable_values.contains("CON"));
    assert(mutable_values.contains("INT"));
    assert(mutable_values.contains("WIS"));
    assert(mutable_values.contains("CHA"));
    implied_values.insert({
        {"STR_MOD", calculate_modifier(mutable_values["STR"])},
        {"DEX_MOD", calculate_modifier(mutable_values["DEX"])},
        {"CON_MOD", calculate_modifier(mutable_values["CON"])},
        {"INT_MOD", calculate_modifier(mutable_values["INT"])},
        {"WIS_MOD", calculate_modifier(mutable_values["WIS"])},
        {"CHA_MOD", calculate_modifier(mutable_values["CHA"])},
    });
}

void Stats::calculate_skill_modifiers() {
    for (const auto& [skill, ability] : get_abilities_for_all_skills()) {
        const std::string ability_modifier = ability + "_MOD";
        assert(implied_values.contains(ability_modifier));
        mutable_values[skill] = implied_values[ability_modifier];
    }
}

} // namespace dnd
