#ifndef CHARACTER_CLASS_HPP_
#define CHARACTER_CLASS_HPP_

#include <string>
#include <vector>

#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class CharacterClass : public FeatureHolder {
public:
    const std::string hit_dice;
    const std::vector<int> asi_levels;
    int subclass_level;
    CharacterClass(
        const std::string& name, std::vector<Feature>&& features, const std::string& hit_dice,
        const std::vector<int>& asi_levels, int subclass_level
    ) noexcept;
};

class CharacterSubclass : public FeatureHolder {
public:
    const std::string class_name;
    CharacterSubclass(const std::string& name, std::vector<Feature>&& features, const std::string& class_name) noexcept;
};

inline CharacterClass::CharacterClass(
    const std::string& name, std::vector<Feature>&& features, const std::string& hit_dice,
    const std::vector<int>& asi_levels, int subclass_level
) noexcept
    : FeatureHolder(name, std::move(features)), hit_dice(hit_dice), asi_levels(asi_levels),
      subclass_level(subclass_level) {}

inline CharacterSubclass::CharacterSubclass(
    const std::string& name, std::vector<Feature>&& features, const std::string& class_name
) noexcept
    : FeatureHolder(name, std::move(features)), class_name(class_name) {}

} // namespace dnd

#endif // CHARACTER_CLASS_HPP_
