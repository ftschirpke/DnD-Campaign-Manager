#ifndef CHARACTER_CLASS_HPP_
#define CHARACTER_CLASS_HPP_

#include <string>
#include <vector>

#include "models/feature_holder.hpp"

namespace dnd {

class CharacterClass : public FeatureHolder {
public:
    const std::string hit_dice;
    const std::vector<int> asi_levels;
    CharacterClass(const std::string& name, const std::string& hit_dice, const std::vector<int>& asi_levels);
};

class CharacterSubclass : public FeatureHolder {
public:
    const std::string class_name;
    CharacterSubclass(const std::string& name, const std::string& class_name);
};

inline CharacterClass::CharacterClass(
    const std::string& name, const std::string& hit_dice, const std::vector<int>& asi_levels
)
    : FeatureHolder(name), hit_dice(hit_dice), asi_levels(asi_levels) {}

inline CharacterSubclass::CharacterSubclass(const std::string& name, const std::string& class_name)
    : FeatureHolder(name), class_name(class_name) {}

} // namespace dnd

#endif // CHARACTER_CLASS_HPP_
