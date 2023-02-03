#ifndef RIV_HOLDER_HPP_
#define RIV_HOLDER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_set>

namespace dnd {

// This class holds a creature's resistances, immunities, and vulnerabilities.
class RIVHolder {
public:
    std::unordered_set<std::string> damage_resistances, damage_immunities, damage_vulnerabilities, condition_immunities;
    bool empty() const;
};

inline bool RIVHolder::empty() const {
    return damage_resistances.empty() && damage_immunities.empty() && damage_vulnerabilities.empty()
           && condition_immunities.empty();
}

} // namespace dnd

#endif // RIV_HOLDER_HPP_
