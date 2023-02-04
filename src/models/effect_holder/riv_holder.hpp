#ifndef RIV_HOLDER_HPP_
#define RIV_HOLDER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_set>

namespace dnd {

/**
 * @brief This class holds a creature's resistances, immunities, and vulnerabilities.
 */
class RIVHolder {
public:
    // the damage types the creature is resistant to
    std::unordered_set<std::string> damage_resistances;
    // the damage types the creature is immune to
    std::unordered_set<std::string> damage_immunities;
    // the damage types the creature is vulnerable to
    std::unordered_set<std::string> damage_vulnerabilities;
    // the conditions the creature is immune to
    std::unordered_set<std::string> condition_immunities;
    /**
     * @brief Returns true if the RIVHolder is empty
     * @return "true" if the RIVHolder is empty, "false" otherwise
     */
    bool empty() const;
};

inline bool RIVHolder::empty() const {
    return damage_resistances.empty() && damage_immunities.empty() && damage_vulnerabilities.empty()
           && condition_immunities.empty();
}

} // namespace dnd

#endif // RIV_HOLDER_HPP_
