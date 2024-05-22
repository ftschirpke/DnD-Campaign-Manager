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
#include <core/utils/data_result.hpp>

namespace dnd {

static int calculate_modifier(int score) { return score / 2 - 5; }

static bool is_ability_score_change(const StatChange& change) {
    const std::string& affected_attribute = change.get_affected_attribute();
    return std::any_of(
        ability_cstrings_inorder.begin(), ability_cstrings_inorder.end(),
        [&affected_attribute](const char* ability_cstring) { return affected_attribute == ability_cstring; }
    );
}

/* static int to_raw(bool value) { return value ? 1 : 0; } */ // TODO: uncomment when needed

static int to_raw(int value) { return value * 100; }

/* static int to_raw(float value) { return value * 100; } */ // TODO: uncomment when needed

Stats Stats::create_default() {
    Stats stats;
    return stats;
}

tl::expected<Stats, Errors> Stats::create(
    int proficiency_bonus, const AbilityScores& base_ability_scores, std::vector<CRef<StatChange>> stat_changes
) {
    Stats stats;
    Errors errors;
    stats.mutable_values["PB"] = to_raw(proficiency_bonus);
    stats.mutable_values["STR_MAX"] = to_raw(20);
    stats.mutable_values["DEX_MAX"] = to_raw(20);
    stats.mutable_values["CON_MAX"] = to_raw(20);
    stats.mutable_values["INT_MAX"] = to_raw(20);
    stats.mutable_values["WIS_MAX"] = to_raw(20);
    stats.mutable_values["CHA_MAX"] = to_raw(20);
    stats.mutable_values["STR"] = to_raw(base_ability_scores.get_strength());
    stats.mutable_values["DEX"] = to_raw(base_ability_scores.get_dexterity());
    stats.mutable_values["CON"] = to_raw(base_ability_scores.get_constitution());
    stats.mutable_values["INT"] = to_raw(base_ability_scores.get_intelligence());
    stats.mutable_values["WIS"] = to_raw(base_ability_scores.get_wisdom());
    stats.mutable_values["CHA"] = to_raw(base_ability_scores.get_charisma());

    for (const StatChange& change : stat_changes) {
        if (is_ability_score_change(change)) {
            errors += change.apply(stats);
        }
    }
    if (!errors.ok()) {
        return tl::unexpected(errors);
    }

    stats.check_maximum_ability_scores();
    stats.calculate_ability_modifiers();
    stats.calculate_ability_save_modifiers();
    stats.calculate_skill_modifiers();

    for (const StatChange& change : stat_changes) {
        if (!is_ability_score_change(change)) {
            errors += change.apply(stats);
        }
    }
    if (!errors.ok()) {
        return tl::unexpected(errors);
    }

    return stats;
}

Stats::Stats() {}

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
    const std::array<const SkillInfo, 18>& all_skill_infos = get_all_skill_infos();
    bool has_skill_modifiers = std::all_of(
        all_skill_infos.begin(), all_skill_infos.end(),
        [this](const SkillInfo& skill_info) { return mutable_values.contains(skill_info.stat_name); }
    );
    return has_ability_modifiers && has_skill_modifiers;
}

std::optional<bool> Stats::get_bool(const std::string& name) const {
    std::optional<int> raw_value = get_raw(name);
    if (!raw_value.has_value()) {
        return std::nullopt;
    }
    return raw_value.value() != 0;
}

std::optional<int> Stats::get_int(const std::string& name) const {
    std::optional<int> raw_value = get_raw(name);
    if (!raw_value.has_value()) {
        return std::nullopt;
    }
    return raw_value.value() / 100;
}

std::optional<float> Stats::get_float(const std::string& name) const {
    std::optional<int> raw_value = get_raw(name);
    if (!raw_value.has_value()) {
        return std::nullopt;
    }
    return raw_value.value() / 100.0f;
}

std::optional<int> Stats::get_raw(const std::string& name) const {
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

std::optional<Ref<int>> Stats::get_raw_mut(const std::string& name) {
    auto it = mutable_values.find(name);
    if (it != mutable_values.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

Ref<int> Stats::get_raw_mut_or_insert(const std::string& name) { return mutable_values[name]; }

int Stats::get_ability_score(Ability ability) const {
    const char* name = ability_cstr_name(ability);
    return get_int(name).value_or(20);
}

int Stats::get_ability_max_score(Ability ability) const {
    const char* max_score_name;
    switch (ability) {
        case Ability::STRENGTH:
            max_score_name = "STR_MAX";
            break;
        case Ability::DEXTERITY:
            max_score_name = "DEX_MAX";
            break;
        case Ability::CONSTITUTION:
            max_score_name = "CON_MAX";
            break;
        case Ability::INTELLIGENCE:
            max_score_name = "INT_MAX";
            break;
        case Ability::WISDOM:
            max_score_name = "WIS_MAX";
            break;
        case Ability::CHARISMA:
            max_score_name = "CHA_MAX";
            break;
    }
    return get_int(max_score_name).value_or(20);
}

int Stats::get_ability_modifier(Ability ability) const {
    std::string modifier_name;
    switch (ability) {
        case Ability::STRENGTH:
            modifier_name = "STR_MOD";
            break;
        case Ability::DEXTERITY:
            modifier_name = "DEX_MOD";
            break;
        case Ability::CONSTITUTION:
            modifier_name = "CON_MOD";
            break;
        case Ability::INTELLIGENCE:
            modifier_name = "INT_MOD";
            break;
        case Ability::WISDOM:
            modifier_name = "WIS_MOD";
            break;
        case Ability::CHARISMA:
            modifier_name = "CHA_MOD";
            break;
    }
    return get_int(modifier_name).value_or(0);
}

int Stats::get_ability_save_modifier(Ability ability) const {
    std::string save_modifier_name;
    switch (ability) {
        case Ability::STRENGTH:
            save_modifier_name = "STR_SAVE";
            break;
        case Ability::DEXTERITY:
            save_modifier_name = "DEX_SAVE";
            break;
        case Ability::CONSTITUTION:
            save_modifier_name = "CON_SAVE";
            break;
        case Ability::INTELLIGENCE:
            save_modifier_name = "INT_SAVE";
            break;
        case Ability::WISDOM:
            save_modifier_name = "WIS_SAVE";
            break;
        case Ability::CHARISMA:
            save_modifier_name = "CHA_SAVE";
            break;
    }
    return get_int(save_modifier_name).value_or(0);
}

void Stats::check_maximum_ability_scores() {
    if (mutable_values["STR"] > mutable_values["STR_MAX"]) {
        mutable_values["STR"] = mutable_values["STR_MAX"];
    }
    if (mutable_values["DEX"] > mutable_values["DEX_MAX"]) {
        mutable_values["DEX"] = mutable_values["DEX_MAX"];
    }
    if (mutable_values["CON"] > mutable_values["CON_MAX"]) {
        mutable_values["CON"] = mutable_values["CON_MAX"];
    }
    if (mutable_values["INT"] > mutable_values["INT_MAX"]) {
        mutable_values["INT"] = mutable_values["INT_MAX"];
    }
    if (mutable_values["WIS"] > mutable_values["WIS_MAX"]) {
        mutable_values["WIS"] = mutable_values["WIS_MAX"];
    }
    if (mutable_values["CHA"] > mutable_values["CHA_MAX"]) {
        mutable_values["CHA"] = mutable_values["CHA_MAX"];
    }
}

void Stats::calculate_ability_modifiers() {
    implied_values.insert({
        {"STR_MOD", to_raw(calculate_modifier(get_ability_score(Ability::STRENGTH)))},
        {"DEX_MOD", to_raw(calculate_modifier(get_ability_score(Ability::DEXTERITY)))},
        {"CON_MOD", to_raw(calculate_modifier(get_ability_score(Ability::CONSTITUTION)))},
        {"INT_MOD", to_raw(calculate_modifier(get_ability_score(Ability::INTELLIGENCE)))},
        {"WIS_MOD", to_raw(calculate_modifier(get_ability_score(Ability::WISDOM)))},
        {"CHA_MOD", to_raw(calculate_modifier(get_ability_score(Ability::CHARISMA)))},
    });
}

void Stats::calculate_ability_save_modifiers() {
    implied_values.insert({
        {"STR_SAVE", to_raw(get_ability_modifier(Ability::STRENGTH))},
        {"DEX_SAVE", to_raw(get_ability_modifier(Ability::DEXTERITY))},
        {"CON_SAVE", to_raw(get_ability_modifier(Ability::CONSTITUTION))},
        {"INT_SAVE", to_raw(get_ability_modifier(Ability::INTELLIGENCE))},
        {"WIS_SAVE", to_raw(get_ability_modifier(Ability::WISDOM))},
        {"CHA_SAVE", to_raw(get_ability_modifier(Ability::CHARISMA))},
    });
}

void Stats::calculate_skill_modifiers() {
    for (const SkillInfo& skill : get_all_skill_infos()) {
        const std::string ability_modifier = fmt::format("{}_MOD", ability_cstr_name(skill.ability));
        mutable_values[skill.stat_name] = to_raw(get_ability_modifier(skill.ability));
    }
}

} // namespace dnd
