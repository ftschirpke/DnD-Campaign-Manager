#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "effect.hpp"

namespace dnd {

class Feature {
public:
    const std::string name, description;
    std::unordered_map<std::string, std::unique_ptr<Effect>> earliest, early, normal, late, latest;
    Feature(const std::string& name, const std::string& description) :
        name(name), description(description) {}
    // TODO: manage adding and removing languages/proficiencies/senses etc.
    // TODO: manage choices (choose one of ...)
    // TODO: manage granting actions, bonus actions, or reactions
};

} // namespace dnd

#endif // FEATURE_HPP_
