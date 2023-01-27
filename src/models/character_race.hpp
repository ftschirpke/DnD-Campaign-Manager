#ifndef RACE_HPP_
#define RACE_HPP_

#include <string>
#include <utility>
#include <vector>

#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class CharacterRace : public FeatureHolder {
public:
    const bool has_subraces;
    CharacterRace(const std::string& name, std::vector<Feature>&& features, const bool has_subraces) noexcept;
};

inline CharacterRace::CharacterRace(
    const std::string& name, std::vector<Feature>&& features, const bool has_subraces
) noexcept
    : FeatureHolder(name, std::move(features)), has_subraces(has_subraces) {}

} // namespace dnd

#endif // RACE_HPP_
