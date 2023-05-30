#ifndef CHARACTER_CLASS_HPP_
#define CHARACTER_CLASS_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "basic_mechanics/dice.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

/**
 * @brief A class representing a character's class.
 */
class CharacterClass : public FeatureHolder {
public:
    /**
     * @brief Constructs a CharacterClass
     * @param name the name of the class
     * @param features a collection of features this class provides to a character
     * @param hit_dice the type of hit dice for this class
     * @param asi_levels the levels at which this class allows ability score inscreases
     * @param subclass_level the level at which a character of this class gains access to subclasses
     */
    CharacterClass(
        const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
        const Dice hit_dice, const std::vector<int>& asi_levels, int subclass_level
    ) noexcept;

    // the type of hit dice for this class
    const Dice hit_dice;
    // the levels at which this class allows ability score inscreases
    const std::vector<int> asi_levels;
    // the level at which a character of this class gains access to subclasses
    int subclass_level;
};

inline CharacterClass::CharacterClass(
    const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
    const Dice hit_dice, const std::vector<int>& asi_levels, int subclass_level
) noexcept
    : FeatureHolder(name, source_file_path, std::move(features)), hit_dice(hit_dice), asi_levels(asi_levels),
      subclass_level(subclass_level) {}

} // namespace dnd

#endif // CHARACTER_CLASS_HPP_
