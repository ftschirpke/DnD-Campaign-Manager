#include <dnd_config.hpp>

#include "decision.hpp"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/effects.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/validation/character/decision/decision_data.hpp>

namespace dnd {

static void ins(std::set<std::string>& set, std::vector<std::string>&& vec) {
    set.insert(std::make_move_iterator(vec.begin()), std::make_move_iterator(vec.end()));
}

Decision Decision::create_for(Data&& data, const Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create Decision from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Decision data is incompatible with the given content.");
    }
    EffectsData res_data(data.get_character_data());
    for (const std::string& stat_change_str : data.selections["stat_changes"]) {
        StatChangeData stat_change_data(data.get_character_data());
        stat_change_data.stat_change_str = stat_change_str;
        res_data.stat_changes_data.emplace_back(std::move(stat_change_data));
    }

    ins(res_data.extra_spells_holder_data.free_cantrips, std::move(data.selections["cantrips_free"]));
    ins(res_data.extra_spells_holder_data.at_will, std::move(data.selections["spells_at_will"]));
    ins(res_data.extra_spells_holder_data.innate, std::move(data.selections["spells_innate"]));
    ins(res_data.extra_spells_holder_data.free_once_a_day, std::move(data.selections["spells_free_once_a_day"]));
    ins(res_data.extra_spells_holder_data.spells_known, std::move(data.selections["spells_known"]));
    ins(res_data.extra_spells_holder_data.spells_known, std::move(data.selections["spells_always_prepared"]));
    ins(res_data.extra_spells_holder_data.spells_known_included, std::move(data.selections["spells_known_included"]));
    ins(res_data.extra_spells_holder_data.added_to_spell_list,
        std::move(data.selections["spells_added_to_spell_list"]));

    ins(res_data.proficiency_holder_data.armor, std::move(data.selections["armor_proficiencies"]));
    ins(res_data.proficiency_holder_data.weapons, std::move(data.selections["weapon_proficiencies"]));
    ins(res_data.proficiency_holder_data.tools, std::move(data.selections["tool_proficiencies"]));
    ins(res_data.proficiency_holder_data.skills, std::move(data.selections["skill_proficiencies"]));
    ins(res_data.proficiency_holder_data.saving_throws, std::move(data.selections["saving_throw_proficiencies"]));
    ins(res_data.proficiency_holder_data.languages, std::move(data.selections["languages"]));
    ins(res_data.proficiency_holder_data.senses, std::move(data.selections["senses"]));

    ins(res_data.riv_holder_data.damage_resistances, std::move(data.selections["damage_resistances"]));
    ins(res_data.riv_holder_data.damage_immunities, std::move(data.selections["damage_immunities"]));
    ins(res_data.riv_holder_data.damage_vulnerabilities, std::move(data.selections["damage_vulnerabilities"]));
    ins(res_data.riv_holder_data.condition_immunities, std::move(data.selections["condition_immunities"]));

    return Decision(data.get_target(), Effects::create_for(std::move(res_data), content));
}

const Effects* Decision::get_target() const noexcept { return target; }

const Effects& Decision::get_effects() const noexcept { return effects; }

Decision::Decision(const Effects* target, Effects effects) noexcept : target(target), effects(std::move(effects)) {}

} // namespace dnd
