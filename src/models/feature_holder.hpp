#ifndef FEATURE_HOLDER_HPP_
#define FEATURE_HOLDER_HPP_

#include <string>
#include <vector>

#include "features/feature.hpp"

namespace dnd {

class FeatureHolder {
public:
    const std::string name;
    std::vector<std::unique_ptr<Feature>> features;
    FeatureHolder(const std::string& name) : name(name) {}
};

} // namespace dnd

#endif // FEATURE_HOLDER_HPP_
