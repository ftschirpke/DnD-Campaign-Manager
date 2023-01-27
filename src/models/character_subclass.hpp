#ifndef CHARACTER_SUBCLASS_HPP_
#define CHARACTER_SUBCLASS_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class CharacterSubclass : public FeatureHolder {
public:
    const std::string class_name;
    CharacterSubclass(const std::string& name, std::vector<Feature>&& features, const std::string& class_name) noexcept;
};

inline CharacterSubclass::CharacterSubclass(
    const std::string& name, std::vector<Feature>&& features, const std::string& class_name
) noexcept
    : FeatureHolder(name, std::move(features)), class_name(class_name) {}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_HPP_
