#include <dnd_config.hpp>

#include "decision.hpp"

#include <set>
#include <string>
#include <utility>
#include <vector>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effect_holder/effect_holder.hpp>
#include <core/models/feature/feature.hpp>
#include <core/validation/character/decision/decision_data.hpp>

void ins(std::set<std::string>& set, std::vector<std::string>&& vec) {
    set.insert(std::make_move_iterator(vec.begin()), std::make_move_iterator(vec.end()));
}

dnd::Decision dnd::Decision::create(dnd::DecisionData&& data, const dnd::ContentHolder& content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create Decision from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("Decision data is incompatible with the given content.");
    }
    EffectHolderData res_data(data.get_character_data());
    for (const auto& effect_str : data.selections["effects"]) {
        EffectData effect_data(data.get_character_data());
        effect_data.effect_str = effect_str;
        res_data.effects_data.emplace_back(std::move(effect_data));
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

    return Decision(data.get_target(), EffectHolder::create(std::move(res_data), content));
}

const dnd::EffectHolder& dnd::Decision::get_effects() const noexcept { return effects; }

dnd::Decision::Decision(const dnd::EffectHolder* target, dnd::EffectHolder effects) noexcept
    : target(target), effects(std::move(effects)) {}
