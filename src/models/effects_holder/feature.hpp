#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include "models/effects_holder/effects_holder.hpp"

namespace dnd {

class Feature : public EffectsHolder {
public:
    bool subclass;
    Feature(const std::string& name, const std::string& description) noexcept;
    Feature(Feature&& other) noexcept = default;
    Feature(EffectsHolder&& effects_holder) noexcept;
    bool isActiveForLevel(int level) const;
};

inline Feature::Feature(const std::string& name, const std::string& description) noexcept
    : EffectsHolder(name, description), subclass(false) {}

inline Feature::Feature(EffectsHolder&& effects_holder) noexcept
    : EffectsHolder(std::move(effects_holder)), subclass(false) {}

} // namespace dnd

#endif // FEATURE_HPP_
