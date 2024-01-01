#ifndef EFFECT_DATA_HPP_
#define EFFECT_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class StatChangeData : public ValidationSubdata {
public:
    StatChangeData(std::shared_ptr<const ValidationData> parent) noexcept;
    std::strong_ordering operator<=>(const StatChangeData&) const noexcept = default;

    std::string stat_change_str;
};

Errors validate_stat_change(const StatChangeData& data);

} // namespace dnd

#endif // EFFECT_DATA_HPP_
