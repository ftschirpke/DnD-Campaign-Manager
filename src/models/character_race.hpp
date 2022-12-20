#ifndef RACE_HPP_
#define RACE_HPP_

#include <string>

#include "models/feature_holder.hpp"

namespace dnd {

class CharacterRace : public FeatureHolder {
public:
    const bool has_subraces;
    CharacterRace(const std::string& name, const bool has_subraces) : FeatureHolder(name), has_subraces(has_subraces) {}
};

class CharacterSubrace : public FeatureHolder {
public:
    const std::string race_name;
    CharacterSubrace(const std::string& name, const std::string& race_name)
        : FeatureHolder(name), race_name(race_name) {}
};

class CharacterSubrace : public FeatureHolder {
public:
    const std::string race_name;
    CharacterSubrace(const std::string& name, const std::string& race_name) :
        FeatureHolder(name), race_name(race_name) {}
};

} // namespace dnd

#endif // RACE_HPP_
