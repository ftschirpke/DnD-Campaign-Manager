#ifndef CHARACTER_SUBRACE_HPP_
#define CHARACTER_SUBRACE_HPP_

#include "dnd_config.hpp"

#include <string>
#include <utility>
#include <vector>

#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class CharacterSubrace : public FeatureHolder {
public:
    const std::string race_name;
    CharacterSubrace(const std::string& name, std::vector<Feature>&& features, const std::string& race_name) noexcept;
};

inline CharacterSubrace::CharacterSubrace(
    const std::string& name, std::vector<Feature>&& features, const std::string& race_name
) noexcept
    : FeatureHolder(name, std::move(features)), race_name(race_name) {}

} // namespace dnd

#endif // CHARACTER_SUBRACE_HPP_
