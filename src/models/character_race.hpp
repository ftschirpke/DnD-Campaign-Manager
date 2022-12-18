#ifndef RACE_HPP_
#define RACE_HPP_

#include <string>

#include "feature_holder.hpp"

namespace dnd {

class CharacterRace : public FeatureHolder {
public:
    const bool has_subraces;
    CharacterRace(const std::string& name, const bool has_subraces) :
        FeatureHolder(name), has_subraces(has_subraces) {}
};

} // namespace dnd

#endif // RACE_HPP_
