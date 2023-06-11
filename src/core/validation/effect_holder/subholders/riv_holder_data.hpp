#ifndef RIV_HOLDER_DATA_HPP_
#define RIV_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <set>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class RIVHolderData : public ValidationSubdata {
public:
    RIVHolderData(const ValidationData* parent) noexcept;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;
    /**
     * @brief Validates the relations of the data to other content pieces with regard to a given content holder
     * @param content the content holder to validate the relations against
     * @return the errors that occured during validation
     */
    virtual Errors validate_relations(const ContentHolder* content) const override;
    /**
     * @brief Checks if the RIV holder is empty
     * @return "true" if the RIV holder is empty, "false" otherwise
     */
    bool empty() const noexcept;

    // the damage types the creature is resistant to
    std::set<std::string> damage_resistances;
    // the damage types the creature is immune to
    std::set<std::string> damage_immunities;
    // the damage types the creature is vulnerable to
    std::set<std::string> damage_vulnerabilities;
    // the conditions the creature is immune to
    std::set<std::string> condition_immunities;
};

} // namespace dnd

#endif // RIV_HOLDER_DATA_HPP_
