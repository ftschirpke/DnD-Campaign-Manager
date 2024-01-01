#ifndef CONDITION_DATA_HPP_
#define CONDITION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ConditionData : public ValidationSubdata {
public:
    ConditionData(std::shared_ptr<const ValidationData> parent) noexcept;
    std::strong_ordering operator<=>(const ConditionData&) const noexcept = default;

    std::string condition_str;
};

Errors validate_condition(const ConditionData& data);

} // namespace dnd

#endif // CONDITION_DATA_HPP_
