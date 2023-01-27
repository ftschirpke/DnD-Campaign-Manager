#ifndef EFFECT_HOLDER_HPP_
#define EFFECT_HOLDER_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/effect_holder/action_holder.hpp"
#include "models/effect_holder/activation.hpp"
#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/extra_spells_holder.hpp"
#include "models/effect_holder/proficiency_holder.hpp"
#include "models/effect_holder/riv_holder.hpp"

namespace dnd {

class EffectHolder {
public:
    // TODO: should these really be public AND non-const?
    std::vector<std::unique_ptr<Activation>> activations;
    ActionHolder actions;
    ExtraSpellsHolder extra_spells;
    ProficiencyHolder proficiencies;
    RIVHolder rivs;
    std::unordered_map<EffectTime, std::vector<std::unique_ptr<const Effect>>> ability_score_effects, normal_effects;
    // TODO: manage choices (choose one of ...)
    bool isActive(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
};


} // namespace dnd

#endif // EFFECT_HOLDER_HPP_
