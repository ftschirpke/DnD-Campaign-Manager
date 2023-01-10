#ifndef EFFECTS_HOLDER_HPP_
#define EFFECTS_HOLDER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "models/effects_holder/action_holder.hpp"
#include "models/effects_holder/activation.hpp"
#include "models/effects_holder/effect.hpp"
#include "models/effects_holder/proficiency_holder.hpp"
#include "models/effects_holder/riv_holder.hpp"

namespace dnd {

class EffectsHolder {
public:
    const std::string name, description;
    // TODO: should these really be public AND non-const?
    std::vector<std::unique_ptr<Activation>> activations;
    ActionHolder actions;
    ProficiencyHolder proficiencies;
    RIVHolder rivs;
    std::unordered_map<EffectTime, std::vector<std::unique_ptr<const Effect>>> ability_score_effects, normal_effects;
    EffectsHolder(const std::string& name, const std::string& description) noexcept;
    EffectsHolder(EffectsHolder&& other) noexcept = default;
    // TODO: manage choices (choose one of ...)
    bool isActive(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
};

inline EffectsHolder::EffectsHolder(const std::string& name, const std::string& description) noexcept
    : name(name), description(description) {}

} // namespace dnd

#endif // EFFECTS_HOLDER_HPP_
