#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "models/features/activation.hpp"
#include "models/features/effect.hpp"

namespace dnd {

class Feature {
public:
    const std::string name, description;
    // TODO: should these really be public AND non-const?
    std::vector<std::unique_ptr<Activation>> activations;
    std::unordered_map<EffectTime, std::vector<std::unique_ptr<const Effect>>> ability_score_effects, normal_effects;
    Feature(const std::string& name, const std::string& description);
    bool isActive(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
    // TODO: manage adding and removing languages/proficiencies/senses etc.
    // TODO: manage choices (choose one of ...)
    // TODO: manage granting actions, bonus actions, or reactions
};

inline Feature::Feature(const std::string& name, const std::string& description)
    : name(name), description(description) {}

} // namespace dnd

#endif // FEATURE_HPP_
