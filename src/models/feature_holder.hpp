#ifndef FEATURE_HOLDER_HPP_
#define FEATURE_HOLDER_HPP_

#include <string>
#include <vector>

#include "models/effects_holder/feature.hpp"

namespace dnd {

class FeatureHolder {
public:
    const std::string name;
    // TODO: should this really be public AND non-const?
    std::vector<Feature> features;
    FeatureHolder(const std::string& name) noexcept;
};

inline FeatureHolder::FeatureHolder(const std::string& name) noexcept : name(name) {}

} // namespace dnd

#endif // FEATURE_HOLDER_HPP_
