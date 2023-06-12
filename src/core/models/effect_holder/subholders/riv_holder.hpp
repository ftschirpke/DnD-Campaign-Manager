#ifndef RIV_HOLDER_HPP_
#define RIV_HOLDER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/controllers/content_holder.hpp>
#include <core/validation/effect_holder/subholders/riv_holder_data.hpp>

namespace dnd {

/**
 * @brief This class holds a creature's resistances, immunities, and vulnerabilities.
 */
class RIVHolder {
public:
    /**
     * @brief Constructs a RIVHolder from the given data and content
     * @param data the data to construct the RIVHolder from
     * @param content the content to use for the resistances, immunities, and vulnerabilities
     * @return the constructed RIVHolder
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static RIVHolder create(RIVHolderData&& data, const ContentHolder* content);

    /**
     * @brief Returns whether the RIVHolder is empty
     * @return "true" if the RIVHolder is empty, "false" otherwise
     */
    bool empty() const;

    const std::vector<std::string>& get_damage_resistances() const noexcept;
    const std::vector<std::string>& get_damage_immunities() const noexcept;
    const std::vector<std::string>& get_damage_vulnerabilities() const noexcept;
    const std::vector<std::string>& get_condition_immunities() const noexcept;
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