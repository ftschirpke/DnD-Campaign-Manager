#include "creature_state.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#include "models/feature_holder.hpp"
#include "models/features/effect.hpp"
#include "models/features/feature.hpp"

void dnd::CreatureState::applyAbilityScoreFeatures(std::shared_ptr<const dnd::FeatureHolder> feature_holder_ptr) {
    // TODO: there must be a better way of doing this than checking for every effect of every feature
    // even if it requires a large refactoring, it might be worth it
    for (const auto& feature_ptr : feature_holder_ptr->features) {
        for (const auto& effect_ptr : feature_ptr->earliest) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
        for (const auto& effect_ptr : feature_ptr->early) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
        for (const auto& effect_ptr : feature_ptr->normal) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
        for (const auto& effect_ptr : feature_ptr->late) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
        for (const auto& effect_ptr : feature_ptr->latest) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
    }
}

void dnd::CreatureState::applyNormalFeatures(std::shared_ptr<const dnd::FeatureHolder> feature_holder_ptr) {
    // TODO: there must be a better way of doing this than checking for every effect of every feature
    // even if it requires a large refactoring, it might be worth it
    for (const auto& feature_ptr : feature_holder_ptr->features) {
        for (const auto& effect_ptr : feature_ptr->earliest) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
        for (const auto& effect_ptr : feature_ptr->early) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
        for (const auto& effect_ptr : feature_ptr->normal) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
        for (const auto& effect_ptr : feature_ptr->late) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
        for (const auto& effect_ptr : feature_ptr->latest) {
            if (isAbility(effect_ptr->affected_attribute)) {
                effect_ptr->apply(attributes, constants);
            }
        }
    }
}

void dnd::CreatureState::determineModifiers() {
    for (const std::string& ability_name : abilities) {
        attributes[ability_name + "MOD"] = modifier(attributes.at(ability_name));
    }
    for (const auto& [skill_name, ability_name] : skills) {
        attributes[skill_name] = attributes.at(ability_name + "MOD");
    }
}
