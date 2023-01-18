#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <string>
#include <vector>

#include "models/effect_holder/effect_holder.hpp"

namespace dnd {

class Feature {
public:
    const std::string name, description;
    bool subclass;
    EffectHolder main_part;
    std::vector<EffectHolder> parts;
    Feature(const std::string& name, const std::string& description) noexcept;
    Feature(Feature&& other) noexcept = default;
    bool isActiveForLevel(int level) const;
};

inline Feature::Feature(const std::string& name, const std::string& description) noexcept
    : name(name), description(description), subclass(false) {}

} // namespace dnd

#endif // FEATURE_HPP_
