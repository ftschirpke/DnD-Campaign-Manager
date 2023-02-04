#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include "dnd_config.hpp"

#include <string>
#include <vector>

#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/effect_holder_with_choices.hpp"

namespace dnd {

/**
 * @brief A class representing a feature provided by classes, races, subclasses and subraces
 */
class Feature {
public:
    /**
     * @brief Constructs a feature with its name and description
     * @param name the name of the feature
     * @param description a human-readable description of what the feature provides
     */
    Feature(const std::string& name, const std::string& description) noexcept;
    Feature(Feature&& other) noexcept = default;
    /**
     * @brief Checks whether the feature is active for a character of a certain level
     * @param level a character level
     * @return "true" if the feature is always active for a character of that level i.e there are no other conditions,
     * "false" otherwise
     */
    bool isActiveForLevel(int level) const;

    // the name of the feature
    const std::string name;
    // a human-readable description of what the feature provides
    const std::string description;
    // set to true, if this is a class feature providing you access to subclasses
    bool subclass;
    // the main part of the feature i.e. the most general part
    EffectHolder main_part;
    // other parts of the feature that activate on later levels or in certain conditions for example
    std::vector<EffectHolder> parts;
    // all parts of the feature that offer a choice to the character
    std::vector<EffectHolderWithChoices> parts_with_choices;
};

inline Feature::Feature(const std::string& name, const std::string& description) noexcept
    : name(name), description(description), subclass(false) {}

} // namespace dnd

#endif // FEATURE_HPP_
