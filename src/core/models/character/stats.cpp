#include <dnd_config.hpp>

#include "stats.hpp"

#include <algorithm>
#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>

#include <fmt/format.h>

#include <core/attribute_names.hpp>
#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/dice.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/utils/data_result.hpp>

namespace dnd {

static int calculate_modifier(int score) { return score / 2 - 5; }

static bool is_ability_score_change(const StatChange& change) {
    const std::string& affected_attribute = change.get_affected_attribute();
    return std::any_of(
        attributes::ABILITIES.begin(), attributes::ABILITIES.end(),
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
    const AbilityScores& base_ability_scores, int proficiency_bonus, std::vector<CRef<StatChange>> stat_changes,
    Dice class_hit_dice, const std::vector<int>& hit_dice_rolls
) {
    Stats stats;
    Errors errors;
    stats.mutable_values[attributes::PROFICIENCY_BONUS] = to_raw(proficiency_bonus);

    stats.mutable_values[attributes::STRENGTH] = to_raw(base_ability_scores.get_strength());
    stats.mutable_values[attributes::DEXTERITY] = to_raw(base_ability_scores.get_dexterity());
    stats.mutable_values[attributes::CONSTITUTION] = to_raw(base_ability_scores.get_constitution());
    stats.mutable_values[attributes::INTELLIGENCE] = to_raw(base_ability_scores.get_intelligence());
    stats.mutable_values[attributes::WISDOM] = to_raw(base_ability_scores.get_wisdom());
    stats.mutable_values[attributes::CHARISMA] = to_raw(base_ability_scores.get_charisma());

    for (const char* ability_name : attributes::ABILITIES) {
        stats.mutable_values[attributes::ability_maximum(ability_name)] = to_raw(20);
    }

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

    const int dex_mod = stats.get_ability_modifier(Ability::DEXTERITY);
    stats.mutable_values[attributes::INITIATIVE] = to_raw(dex_mod);
    stats.mutable_values[attributes::ARMOR_CLASS] = to_raw(10 + dex_mod);

    const int con_mod = stats.get_ability_modifier(Ability::CONSTITUTION);
    int max_hp = class_hit_dice.max_value();
    for (const int hit_dice_roll : hit_dice_rolls) {
        max_hp += hit_dice_roll + con_mod;
    }
    stats.mutable_values[attributes::MAXIMUM_HP] = to_raw(max_hp);

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
    assert(mutable_values.contains(attributes::STRENGTH));
    assert(mutable_values.contains(attributes::DEXTERITY));
    assert(mutable_values.contains(attributes::CONSTITUTION));
    assert(mutable_values.contains(attributes::INTELLIGENCE));
    assert(mutable_values.contains(attributes::WISDOM));
    assert(mutable_values.contains(attributes::CHARISMA));

    bool has_ability_modifiers = true;
    for (const char* ability_name : attributes::ABILITIES) {
        has_ability_modifiers &= mutable_values.contains(attributes::ability_modifier(ability_name));
    }

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
    /* auto const_it = constant_values.find(name); */
    /* if (const_it != constant_values.end()) { */
    /*     return const_it->second; */
    /* } */
    auto it = mutable_values.find(name);
    if (it != mutable_values.end()) {
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

int Stats::get_current_hp() const {
    // TODO: implement
    return get_maximum_hp();
}

int Stats::get_maximum_hp() const { return get_int(attributes::MAXIMUM_HP).value_or(0); }

int Stats::get_speed() const { return get_int(attributes::SPEED).value_or(0); }

int Stats::get_armor_class() const { return get_int(attributes::ARMOR_CLASS).value_or(10); }

int Stats::get_initiative() const {
    return get_int(attributes::INITIATIVE).value_or(get_ability_modifier(Ability::DEXTERITY));
}

int Stats::get_skill_modifier(Skill skill) const {
    const std::optional<SkillInfo> skill_info_opt = get_skill_info(skill);
    if (!skill_info_opt.has_value()) {
        return 0;
    }
    const SkillInfo& skill_info = skill_info_opt.value();
    return get_int(skill_info.stat_name).value_or(0);
}

int Stats::get_ability_score(Ability ability) const {
    const char* name = ability_cstr_name(ability);
    return get_int(name).value_or(10);
}

int Stats::get_ability_max_score(Ability ability) const {
    const char* ability_name = ability_cstr_name(ability);
    const std::string max_score_name = attributes::ability_maximum(ability_name);
    return get_int(max_score_name).value_or(20);
}

int Stats::get_ability_modifier(Ability ability) const {
    const char* ability_name = ability_cstr_name(ability);
    const std::string modifier_name = attributes::ability_modifier(ability_name);
    return get_int(modifier_name).value_or(0);
}

int Stats::get_ability_save_modifier(Ability ability) const {
    const char* ability_name = ability_cstr_name(ability);
    const std::string save_modifier_name = attributes::ability_saving_throw(ability_name);
    return get_int(save_modifier_name).value_or(0);
}

void Stats::check_maximum_ability_scores() {
    for (const char* ability_name : attributes::ABILITIES) {
        if (mutable_values[ability_name] > mutable_values[attributes::ability_maximum(ability_name)]) {
            mutable_values[ability_name] = mutable_values[attributes::ability_maximum(ability_name)];
        }
    }
}

void Stats::calculate_ability_modifiers() {
    for (Ability ability : abilities_inorder) {
        const char* ability_name = ability_cstr_name(ability);
        const std::string ability_modifier = attributes::ability_modifier(ability_name);
        mutable_values[ability_modifier] = to_raw(calculate_modifier(get_ability_score(ability)));
    }
}

void Stats::calculate_ability_save_modifiers() {
    for (Ability ability : abilities_inorder) {
        const char* ability_name = ability_cstr_name(ability);
        const std::string ability_save = attributes::ability_saving_throw(ability_name);
        mutable_values[ability_save] = to_raw(get_ability_modifier(ability));
    }
}

void Stats::calculate_skill_modifiers() {
    for (const SkillInfo& skill : get_all_skill_infos()) {
        const char* ability_name = ability_cstr_name(skill.ability);
        const std::string ability_modifier = attributes::ability_modifier(ability_name);
        mutable_values[skill.stat_name] = to_raw(get_ability_modifier(skill.ability));
    }
}

} // namespace dnd
