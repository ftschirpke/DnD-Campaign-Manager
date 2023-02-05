#ifndef FEATURE_HOLDER_HPP_
#define FEATURE_HOLDER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "models/effect_holder/feature.hpp"

namespace dnd {

/**
 * @brief A class representing all types of bodies that provide features to characters i.e. (sub)classes and (sub)races
 */
class FeatureHolder {
public:
    /**
     * @brief Constructs a feature holder with a name and with features
     * @param name the name of the feature holder
     * @param features a collection of features this holder provides to a character of its type
     */
    FeatureHolder(const std::string& name, std::vector<Feature>&& features) noexcept;

    // the name of the feature holder
    const std::string name;
    // a collection of features this holder provides to a character of its type
    const std::vector<Feature> features;
};

inline FeatureHolder::FeatureHolder(const std::string& name, std::vector<Feature>&& features) noexcept
    : name(name), features(std::move(features)) {}

} // namespace dnd

#endif // FEATURE_HOLDER_HPP_
