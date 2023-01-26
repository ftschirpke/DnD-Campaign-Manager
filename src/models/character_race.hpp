#ifndef RACE_HPP_
#define RACE_HPP_

#include <string>
#include <vector>

#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class CharacterRace : public FeatureHolder {
public:
    const bool has_subraces;
    CharacterRace(const std::string& name, std::vector<Feature>&& features, const bool has_subraces) noexcept;
};

class CharacterSubrace : public FeatureHolder {
public:
    const std::string race_name;
    CharacterSubrace(const std::string& name, std::vector<Feature>&& features, const std::string& race_name) noexcept;
};

inline CharacterRace::CharacterRace(
    const std::string& name, std::vector<Feature>&& features, const bool has_subraces
) noexcept
    : FeatureHolder(name, std::move(features)), has_subraces(has_subraces) {}

inline CharacterSubrace::CharacterSubrace(
    const std::string& name, std::vector<Feature>&& features, const std::string& race_name
) noexcept
    : FeatureHolder(name, std::move(features)), race_name(race_name) {}

} // namespace dnd

#endif // RACE_HPP_
