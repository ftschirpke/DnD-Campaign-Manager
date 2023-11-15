#ifndef RIV_HOLDER_DATA_HPP_
#define RIV_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class RIVHolderData : public ValidationSubdata {
public:
    RIVHolderData(const ValidationData* parent) noexcept;
    std::strong_ordering operator<=>(const RIVHolderData&) const noexcept = default;
    virtual Errors validate() const override;
    virtual Errors validate_relations(const Content& content) const override;
    bool empty() const noexcept;

    std::set<std::string> damage_resistances;
    std::set<std::string> damage_immunities;
    std::set<std::string> damage_vulnerabilities;
    std::set<std::string> condition_immunities;
};

} // namespace dnd

#endif // RIV_HOLDER_DATA_HPP_
