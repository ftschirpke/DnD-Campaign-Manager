#include <dnd_config.hpp>

#include "action_holder.hpp"

#include <map>
#include <string>
#include <utility>

#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/effects/subholders/action_holder_data.hpp>

dnd::ActionHolder dnd::ActionHolder::create(dnd::ActionHolderData&& data) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create ActionHolder from invalid data.");
    }
    return ActionHolder(std::move(data.actions), std::move(data.bonus_actions), std::move(data.reactions));
}

const std::map<std::string, std::string>& dnd::ActionHolder::get_actions() const noexcept { return actions; }

const std::map<std::string, std::string>& dnd::ActionHolder::get_bonus_actions() const noexcept {
    return bonus_actions;
}

const std::map<std::string, std::string>& dnd::ActionHolder::get_reactions() const noexcept { return reactions; }

bool dnd::ActionHolder::empty() const { return actions.empty() && bonus_actions.empty() && reactions.empty(); }

void dnd::ActionHolder::merge(dnd::ActionHolder&& other) {
    actions.merge(std::move(other.actions));
    bonus_actions.merge(std::move(other.bonus_actions));
    reactions.merge(std::move(other.reactions));
}

dnd::ActionHolder::ActionHolder(
    std::map<std::string, std::string>&& actions, std::map<std::string, std::string>&& bonus_actions,
    std::map<std::string, std::string>&& reactions
) noexcept
    : actions(std::move(actions)), bonus_actions(std::move(bonus_actions)), reactions(std::move(reactions)) {}
