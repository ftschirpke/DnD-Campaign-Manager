#ifndef RACE_HPP_
#define RACE_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

/**
 * @brief A class representing a character's race.
 */
class CharacterRace : public FeatureHolder {
public:
    /**
     * @brief Constructs a CharacterRace
     * @param name the name of the race
     * @param features a collection of features this race provides to a character
     * @param has_subraces "true" if this race has subraces, "false" otherwise
     */
    CharacterRace(const std::string& name, std::vector<Feature>&& features, const bool has_subraces) noexcept;

    // "true" if this race has subraces, "false" otherwise
    const bool has_subraces;
};

inline CharacterRace::CharacterRace(
    const std::string& name, std::vector<Feature>&& features, const bool has_subraces
) noexcept
    : FeatureHolder(name, std::move(features)), has_subraces(has_subraces) {}

} // namespace dnd

#endif // RACE_HPP_
