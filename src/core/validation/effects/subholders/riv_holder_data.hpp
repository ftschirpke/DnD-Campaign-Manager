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
    RIVHolderData(std::shared_ptr<const ValidationData> parent) noexcept;
    std::strong_ordering operator<=>(const RIVHolderData&) const noexcept = default;
    bool empty() const noexcept;

    std::set<std::string> damage_resistances;
    std::set<std::string> damage_immunities;
    std::set<std::string> damage_vulnerabilities;
    std::set<std::string> condition_immunities;
};

Errors validate_riv_holder_for_content(const RIVHolderData& data, const Content& content);

} // namespace dnd

#endif // RIV_HOLDER_DATA_HPP_
