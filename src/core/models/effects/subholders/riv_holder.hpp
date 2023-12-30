#ifndef RIV_HOLDER_HPP_
#define RIV_HOLDER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/validation/effects/subholders/riv_holder_data.hpp>

namespace dnd {

class Content;

/**
 * @brief This class holds a creature's resistances, immunities, and vulnerabilities.
 */
class RIVHolder {
public:
    using Data = RIVHolderData;

    static CreateResult<RIVHolder> create_for(Data&& data, const Content& content);

    const std::vector<std::string>& get_damage_resistances() const noexcept;
    const std::vector<std::string>& get_damage_immunities() const noexcept;
    const std::vector<std::string>& get_damage_vulnerabilities() const noexcept;
    const std::vector<std::string>& get_condition_immunities() const noexcept;

    bool empty() const;
    void merge(RIVHolder&& other);
private:
    RIVHolder(
        std::vector<std::string>&& damage_resistances, std::vector<std::string>&& damage_immunities,
        std::vector<std::string>&& damage_vulnerabilities, std::vector<std::string>&& condition_immunities
    ) noexcept;

    std::vector<std::string> damage_resistances;
    std::vector<std::string> damage_immunities;
    std::vector<std::string> damage_vulnerabilities;
    std::vector<std::string> condition_immunities;
};

} // namespace dnd

#endif // RIV_HOLDER_HPP_
