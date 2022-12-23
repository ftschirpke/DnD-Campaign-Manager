#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "models/features/effect.hpp"

namespace dnd {

class Feature {
public:
    const std::string name, description;
    // TODO: should this really be public AND non-const?
    std::unordered_map<EffectTime, std::vector<std::unique_ptr<const Effect>>> ability_score_effects, normal_effects;
    Feature(const std::string& name, const std::string& description) : name(name), description(description) {}
    // TODO: manage adding and removing languages/proficiencies/senses etc.
    // TODO: manage choices (choose one of ...)
    // TODO: manage granting actions, bonus actions, or reactions
    // TODO: activation
};

} // namespace dnd

#endif // FEATURE_HPP_
