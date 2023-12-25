#include <dnd_config.hpp>

#include "effects.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/condition/condition_factory.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/models/effects/stat_change/stat_change_factory.hpp>
#include <core/models/effects/subholders/action_holder.hpp>
#include <core/models/effects/subholders/extra_spells_holder.hpp>
#include <core/models/effects/subholders/proficiency_holder.hpp>
#include <core/models/effects/subholders/riv_holder.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/effects/subholders/action_holder_data.hpp>
#include <core/validation/effects/subholders/extra_spells_holder_data.hpp>
#include <core/validation/effects/subholders/proficiency_holder_data.hpp>
#include <core/validation/effects/subholders/riv_holder_data.hpp>

dnd::Effects dnd::Effects::create(dnd::EffectsData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create effects from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("Effects data is incompatible with the given content.");
    }
    std::vector<std::unique_ptr<Condition>> activation_conditions;
    activation_conditions.reserve(data.activation_conditions_data.size());
    for (ConditionData& condition_data : data.activation_conditions_data) {
        activation_conditions.emplace_back(create_condition(std::move(condition_data)));
    }
    std::vector<Choice> choices;
    choices.reserve(data.choices_data.size());
    for (ChoiceData& choice_data : data.choices_data) {
        choices.emplace_back(Choice::create(std::move(choice_data), content));
    }
    std::vector<std::unique_ptr<StatChange>> stat_changes;
    stat_changes.reserve(data.stat_changes_data.size());
    for (StatChangeData& stat_change_data : data.stat_changes_data) {
        stat_changes.emplace_back(create_stat_change(std::move(stat_change_data)));
    }
    return Effects(
        std::move(activation_conditions), std::move(choices), std::move(stat_changes),
        ActionHolder::create(std::move(data.action_holder_data)),
        ExtraSpellsHolder::create(std::move(data.extra_spells_holder_data), content),
        ProficiencyHolder::create(std::move(data.proficiency_holder_data), content),
        RIVHolder::create(std::move(data.riv_holder_data), content)
    );
}

dnd::Effects::Effects(
    std::vector<std::unique_ptr<Condition>>&& activation_conditions, std::vector<Choice>&& choices,
    std::vector<std::unique_ptr<StatChange>>&& stat_changes, ActionHolder&& action_holder,
    ExtraSpellsHolder&& extra_spells_holder, ProficiencyHolder&& proficiency_holder, RIVHolder&& riv_holder
) noexcept
    : activation_conditions(std::move(activation_conditions)), choices(std::move(choices)),
      stat_changes(std::move(stat_changes)), actions(std::move(action_holder)),
      extra_spells(std::move(extra_spells_holder)), proficiencies(std::move(proficiency_holder)),
      rivs(std::move(riv_holder)) {}

const std::vector<std::unique_ptr<dnd::Condition>>& dnd::Effects::get_activation_conditions() const noexcept {
    return activation_conditions;
}

const std::vector<dnd::Choice>& dnd::Effects::get_choices() const noexcept { return choices; }

const std::vector<std::unique_ptr<dnd::StatChange>>& dnd::Effects::get_stat_changes() const noexcept {
    return stat_changes;
}

const dnd::ActionHolder& dnd::Effects::get_actions() const noexcept { return actions; }

const dnd::ExtraSpellsHolder& dnd::Effects::get_extra_spells() const noexcept { return extra_spells; }

const dnd::ProficiencyHolder& dnd::Effects::get_proficiencies() const noexcept { return proficiencies; }

const dnd::RIVHolder& dnd::Effects::get_rivs() const noexcept { return rivs; }

bool dnd::Effects::empty() const {
    return activation_conditions.empty() && stat_changes.empty() && actions.empty() && extra_spells.empty()
           && proficiencies.empty() && rivs.empty();
}

bool dnd::Effects::is_active(
    const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    for (const std::unique_ptr<Condition>& condition : activation_conditions) {
        if (!condition->evaluate(attributes, constants)) {
            return false;
        }
    }
    return true;
}

void dnd::Effects::merge(dnd::Effects&& other) {
    activation_conditions.insert(
        activation_conditions.end(), std::make_move_iterator(other.activation_conditions.begin()),
        std::make_move_iterator(other.activation_conditions.end())
    );
    choices.insert(
        choices.end(), std::make_move_iterator(other.choices.begin()), std::make_move_iterator(other.choices.end())
    );
    stat_changes.insert(
        stat_changes.end(), std::make_move_iterator(other.stat_changes.begin()),
        std::make_move_iterator(other.stat_changes.end())
    );
    actions.merge(std::move(other.actions));
    extra_spells.merge(std::move(other.extra_spells));
    proficiencies.merge(std::move(other.proficiencies));
    rivs.merge(std::move(other.rivs));
}
