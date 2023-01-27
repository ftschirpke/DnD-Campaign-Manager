#ifndef FEATURE_HOLDER_HPP_
#define FEATURE_HOLDER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "models/effect_holder/feature.hpp"

namespace dnd {

class FeatureHolder {
public:
    const std::string name;
    const std::vector<Feature> features;
    FeatureHolder(const std::string& name, std::vector<Feature>&& features) noexcept;
};

inline FeatureHolder::FeatureHolder(const std::string& name, std::vector<Feature>&& features) noexcept
    : name(name), features(std::move(features)) {}

} // namespace dnd

#endif // FEATURE_HOLDER_HPP_
