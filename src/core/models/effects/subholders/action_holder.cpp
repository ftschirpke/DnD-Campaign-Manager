#include <dnd_config.hpp>

#include "action_holder.hpp"

#include <map>
#include <string>
#include <utility>

#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/effects/subholders/action_holder_data.hpp>

namespace dnd {

ActionHolder ActionHolder::create(ActionHolderData&& data) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create ActionHolder from invalid data.");
    }
    return ActionHolder(std::move(data.actions), std::move(data.bonus_actions), std::move(data.reactions));
}

const std::map<std::string, std::string>& ActionHolder::get_actions() const noexcept { return actions; }

const std::map<std::string, std::string>& ActionHolder::get_bonus_actions() const noexcept {
    return bonus_actions;
}

const std::map<std::string, std::string>& ActionHolder::get_reactions() const noexcept { return reactions; }

bool ActionHolder::empty() const { return actions.empty() && bonus_actions.empty() && reactions.empty(); }

void ActionHolder::merge(ActionHolder&& other) {
    actions.merge(std::move(other.actions));
    bonus_actions.merge(std::move(other.bonus_actions));
    reactions.merge(std::move(other.reactions));
}

ActionHolder::ActionHolder(
    std::map<std::string, std::string>&& actions, std::map<std::string, std::string>&& bonus_actions,
    std::map<std::string, std::string>&& reactions
) noexcept
    : actions(std::move(actions)), bonus_actions(std::move(bonus_actions)), reactions(std::move(reactions)) {}

} // namespace dnd
