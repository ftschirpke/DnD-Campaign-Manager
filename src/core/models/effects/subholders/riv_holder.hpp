#ifndef RIV_HOLDER_HPP_
#define RIV_HOLDER_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>
#include <string>
#include <vector>

#include <core/data_result.hpp>

namespace dnd {

class Content;

/**
 * @brief This class holds a creature's resistances, immunities, and vulnerabilities.
 */
class RIVHolder {
public:
    struct Data;

    static CreateResult<RIVHolder> create_for(Data&& data, const Content& content);

    const std::vector<std::string>& get_damage_resistances() const;
    const std::vector<std::string>& get_damage_immunities() const;
    const std::vector<std::string>& get_damage_vulnerabilities() const;
    const std::vector<std::string>& get_condition_immunities() const;

    bool empty() const;
    void merge(RIVHolder&& other);
private:
    RIVHolder(
        std::vector<std::string>&& damage_resistances, std::vector<std::string>&& damage_immunities,
        std::vector<std::string>&& damage_vulnerabilities, std::vector<std::string>&& condition_immunities
    );

    std::vector<std::string> damage_resistances;
    std::vector<std::string> damage_immunities;
    std::vector<std::string> damage_vulnerabilities;
    std::vector<std::string> condition_immunities;
};

struct RIVHolder::Data {
    std::strong_ordering operator<=>(const RIVHolder::Data&) const = default;
    bool empty() const;

    std::set<std::string> damage_resistances;
    std::set<std::string> damage_immunities;
    std::set<std::string> damage_vulnerabilities;
    std::set<std::string> condition_immunities;
};

inline bool RIVHolder::Data::empty() const {
    return damage_resistances.empty() && damage_immunities.empty() && damage_vulnerabilities.empty()
           && condition_immunities.empty();
}


} // namespace dnd

#endif // RIV_HOLDER_HPP_
