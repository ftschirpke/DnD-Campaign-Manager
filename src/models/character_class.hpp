#ifndef CHARACTER_CLASS_HPP_
#define CHARACTER_CLASS_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "basic_mechanics/dice.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class CharacterClass : public FeatureHolder {
public:
    const Dice hit_dice;
    const std::vector<int> asi_levels;
    int subclass_level;
    CharacterClass(
        const std::string& name, std::vector<Feature>&& features, const Dice hit_dice,
        const std::vector<int>& asi_levels, int subclass_level
    ) noexcept;
};

inline CharacterClass::CharacterClass(
    const std::string& name, std::vector<Feature>&& features, const Dice hit_dice, const std::vector<int>& asi_levels,
    int subclass_level
) noexcept
    : FeatureHolder(name, std::move(features)), hit_dice(hit_dice), asi_levels(asi_levels),
      subclass_level(subclass_level) {}

} // namespace dnd

#endif // CHARACTER_CLASS_HPP_
