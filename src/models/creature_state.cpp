#include "dnd_config.hpp"

#include "creature_state.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/feature_holder.hpp"
#include "models/features/effect.hpp"
#include "models/features/feature.hpp"

void dnd::CreatureState::applyAbilityScoreEffects() {
    for (const auto& effect_time : effect_times_in_order) {
        for (const auto& feature_ptr : active_features) {
            if (feature_ptr->ability_score_effects.find(effect_time) != feature_ptr->ability_score_effects.cend()) {
                for (const auto& effect_ptr : feature_ptr->ability_score_effects.at(effect_time)) {
                    effect_ptr->applyTo(attributes, constants);
                }
            }
        }
    }
}

void dnd::CreatureState::applyNormalEffects() {
    for (const auto& effect_time : effect_times_in_order) {
        for (const auto& feature_ptr : active_features) {
            if (feature_ptr->normal_effects.find(effect_time) != feature_ptr->normal_effects.cend()) {
                for (const auto& effect_ptr : feature_ptr->normal_effects.at(effect_time)) {
                    effect_ptr->applyTo(attributes, constants);
                }
            }
        }
    }
}

void dnd::CreatureState::determineModifiers() {
    for (const std::string& ability_name : abilities) {
        attributes[ability_name + "MOD"] = modifier(attributes.at(ability_name));
        attributes[ability_name + "SAVE"] = modifier(attributes.at(ability_name));
    }
    for (const auto& [skill_name, ability_name] : skills) {
        attributes[skill_name] = attributes.at(ability_name + "MOD");
    }
}

void dnd::CreatureState::addFeatureHolder(const FeatureHolder* const feature_holder_ptr) {
    if (feature_holder_ptr == nullptr) {
        return;
    }
    for (const auto& feature : feature_holder_ptr->features) {
        if (feature.isActive(attributes, constants)) {
            active_features.emplace_back(&feature);
        }
    }
}

void dnd::CreatureState::calculate() {
    applyAbilityScoreEffects();
    determineModifiers();
    applyNormalEffects();
}
