#include "dnd_config.hpp"

#include "character_state.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "basic_mechanics/abilities.hpp"
#include "basic_mechanics/skills.hpp"
#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/effect_holder_with_choices.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

void dnd::CharacterState::applyAbilityScoreEffects() {
    for (const auto& effect_time : effect_times_in_order) {
        for (const auto& effect_holder_ptr : active_effect_holders) {
            if (effect_holder_ptr->ability_score_effects.contains(effect_time)) {
                for (const auto& effect_ptr : effect_holder_ptr->ability_score_effects.at(effect_time)) {
                    effect_ptr->applyTo(attributes, constants);
                }
            }
        }
    }
    for (const auto& ability : ability_strings_inorder) {
        attributes[ability] = std::min(attributes[ability], attributes[ability + "MAX"]);
    }
}

void dnd::CharacterState::applyNormalEffects() {
    for (const auto& effect_time : effect_times_in_order) {
        for (const auto& effect_holder_ptr : active_effect_holders) {
            if (effect_holder_ptr->normal_effects.contains(effect_time)) {
                for (const auto& effect_ptr : effect_holder_ptr->normal_effects.at(effect_time)) {
                    effect_ptr->applyTo(attributes, constants);
                }
            }
        }
    }
}

void dnd::CharacterState::determineModifiers() {
    for (const std::string& ability_name : ability_strings_inorder) {
        attributes[ability_name + "MOD"] = modifier(attributes.at(ability_name));
        attributes[ability_name + "SAVE"] = modifier(attributes.at(ability_name));
    }
    for (const auto& [skill_name, ability_name] : skill_abilities) {
        attributes[skill_name] = attributes.at(ability_name + "MOD");
    }
}

void dnd::CharacterState::addFeatureHolder(const FeatureHolder* const feature_holder_ptr) {
    if (feature_holder_ptr == nullptr) {
        return;
    }
    for (const auto& feature : feature_holder_ptr->features) {
        if (addEffectHolder(feature.main_part)) {
            for (const auto& eh : feature.parts) {
                addEffectHolder(eh);
            }
            for (const auto& eh_with_choice : feature.parts_with_choices) {
                addEffectHolderWithChoices(eh_with_choice);
            }
        }
    }
}

bool dnd::CharacterState::addEffectHolder(const dnd::EffectHolder& effect_holder) {
    if (!effect_holder.isActive(attributes, constants)) {
        return false;
    }

    active_effect_holders.emplace_back(&effect_holder);

    return true;
}

bool dnd::CharacterState::addEffectHolderWithChoices(const dnd::EffectHolderWithChoices& eh_with_choice) {
    if (!eh_with_choice.isActive(attributes, constants)) {
        return false;
    }

    for (auto it = eh_with_choice.choices.cbegin(); it != eh_with_choice.choices.cend(); ++it) {
        auto is_decision_for_choice = [&](const CharacterDecision& decision) { return decision.choice == it->get(); };
        const auto decision_it = std::find_if(decisions.cbegin(), decisions.cend(), is_decision_for_choice);
        if (decision_it != decisions.cend()) {
            active_effect_holders.emplace_back(&decision_it->decision_effects);
        } else {
            std::cerr << "Warning: You still have to choose " << it->get()->amount << " values for \""
                      << it->get()->attribute_name << "\"\n";
        }
    }

    return true;
}

void dnd::CharacterState::calculate() {
    applyAbilityScoreEffects();
    determineModifiers();
    // TODO: calculate MAXHP
    applyNormalEffects();
}
