#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "models/features/action_holder.hpp"
#include "models/features/activation.hpp"
#include "models/features/effect.hpp"
#include "models/features/proficiency_holder.hpp"
#include "models/features/riv_holder.hpp"

namespace dnd {

class Feature {
public:
    const std::string name, description;
    bool subclass;
    // TODO: should these really be public AND non-const?
    std::vector<std::unique_ptr<Activation>> activations;
    ActionHolder actions;
    ProficiencyHolder proficiencies;
    RIVHolder rivs;
    std::unordered_map<EffectTime, std::vector<std::unique_ptr<const Effect>>> ability_score_effects, normal_effects;
    Feature(Feature&& other) = default;
    Feature(const std::string& name, const std::string& description);
    bool isActive(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
    bool isActiveForLevel(int level) const;
    // TODO: manage adding and removing languages/proficiencies/senses etc.
    // TODO: manage choices (choose one of ...)
    // TODO: manage granting actions, bonus actions, or reactions
};

inline Feature::Feature(const std::string& name, const std::string& description)
    : name(name), description(description), subclass(false) {}

} // namespace dnd

#endif // FEATURE_HPP_
