#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include "models/effect_holder/effect_holder.hpp"

namespace dnd {

class Feature : public EffectHolder {
public:
    bool subclass;
    Feature(const std::string& name, const std::string& description) noexcept;
    Feature(Feature&& other) noexcept = default;
    Feature(EffectHolder&& effect_holder) noexcept;
    bool isActiveForLevel(int level) const;
};

inline Feature::Feature(const std::string& name, const std::string& description) noexcept
    : EffectHolder(name, description), subclass(false) {}

inline Feature::Feature(EffectHolder&& effect_holder) noexcept
    : EffectHolder(std::move(effect_holder)), subclass(false) {}

} // namespace dnd

#endif // FEATURE_HPP_
