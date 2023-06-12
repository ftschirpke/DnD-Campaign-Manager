#include <dnd_config.hpp>

#include "effect_holder.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effect_holder/condition/condition.hpp>
#include <core/models/effect_holder/condition/condition_factory.hpp>
#include <core/models/effect_holder/effect/effect.hpp>
#include <core/models/effect_holder/effect/effect_factory.hpp>
#include <core/models/effect_holder/subholders/action_holder.hpp>
#include <core/models/effect_holder/subholders/extra_spells_holder.hpp>
#include <core/models/effect_holder/subholders/proficiency_holder.hpp>
#include <core/models/effect_holder/subholders/riv_holder.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>
#include <core/validation/effect_holder/subholders/action_holder_data.hpp>
#include <core/validation/effect_holder/subholders/extra_spells_holder_data.hpp>
#include <core/validation/effect_holder/subholders/proficiency_holder_data.hpp>
#include <core/validation/effect_holder/subholders/riv_holder_data.hpp>

dnd::EffectHolder dnd::EffectHolder::create(dnd::EffectHolderData&& data, const dnd::ContentHolder* content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create effect holder from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("ExtraSpellsHolderData is incompatible with the given content.");
    }
    std::vector<std::unique_ptr<Condition>> activation_conditions;
    activation_conditions.reserve(data.activation_conditions_data.size());
    for (auto& condition_data : data.activation_conditions_data) {
        activation_conditions.push_back(std::make_unique<Condition>(create_condition(std::move(condition_data))));
    }
    std::vector<std::unique_ptr<dnd::Effect>> effects;
    effects.reserve(data.effects_data.size());
    for (auto& effect_data : data.effects_data) {
        effects.push_back(std::make_unique<dnd::Effect>(create_effect(std::move(effect_data))));
    }
    return EffectHolder(
        std::move(activation_conditions), std::move(effects), ActionHolder::create(std::move(data.action_holder_data)),
        ExtraSpellsHolder::create(std::move(data.extra_spells_holder_data), content),
        ProficiencyHolder::create(std::move(data.proficiency_holder_data), content),
        RIVHolder::create(std::move(data.riv_holder_data), content)
    );
}

dnd::EffectHolder::EffectHolder(
    std::vector<std::unique_ptr<Condition>>&& activation_conditions, std::vector<std::unique_ptr<Effect>>&& effects,
    ActionHolder&& action_holder, ExtraSpellsHolder&& extra_spells_holder, ProficiencyHolder&& proficiency_holder,
    RIVHolder&& riv_holder
) noexcept
    : activation_conditions(std::move(activation_conditions)), effects(std::move(effects)),
      actions(std::move(action_holder)), extra_spells(std::move(extra_spells_holder)),
      proficiencies(std::move(proficiency_holder)), rivs(std::move(riv_holder)) {}

const std::vector<std::unique_ptr<dnd::Condition>>& dnd::EffectHolder::get_activation_conditions() const noexcept {
    return activation_conditions;
}

const std::vector<std::unique_ptr<dnd::Effect>>& dnd::EffectHolder::get_effects() const noexcept { return effects; }

const dnd::ActionHolder& dnd::EffectHolder::get_actions() const noexcept { return actions; }

const dnd::ExtraSpellsHolder& dnd::EffectHolder::get_extra_spells() const noexcept { return extra_spells; }

const dnd::ProficiencyHolder& dnd::EffectHolder::get_proficiencies() const noexcept { return proficiencies; }

const dnd::RIVHolder& dnd::EffectHolder::get_rivs() const noexcept { return rivs; }

bool dnd::EffectHolder::empty() const {
    return activation_conditions.empty() && effects.empty() && actions.empty() && extra_spells.empty()
           && proficiencies.empty() && rivs.empty();
}

bool dnd::EffectHolder::is_active(
    const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    for (auto& condition : activation_conditions) {
        if (!condition->evaluate(attributes, constants)) {
            return false;
        }
    }
    return true;
}
