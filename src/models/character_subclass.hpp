#ifndef CHARACTER_SUBCLASS_HPP_
#define CHARACTER_SUBCLASS_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

/**
 * @brief A class representing a character's subclass
 */
class CharacterSubclass : public FeatureHolder {
public:
    /**
     * @brief Constructs a CharacterSubclass
     * @param name the name of the subclass
     * @param features the collection of features this subclass provides to a character
     * @param class_name the name of the class this is a subclass of
     */
    CharacterSubclass(const std::string& name, std::vector<Feature>&& features, const std::string& class_name) noexcept;

    // the name of the class this is a subclass of
    const std::string class_name;
};

inline CharacterSubclass::CharacterSubclass(
    const std::string& name, std::vector<Feature>&& features, const std::string& class_name
) noexcept
    : FeatureHolder(name, std::move(features)), class_name(class_name) {}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_HPP_
