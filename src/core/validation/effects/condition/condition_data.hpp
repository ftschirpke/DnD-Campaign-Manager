#ifndef CONDITION_DATA_HPP_
#define CONDITION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>

namespace dnd {

class ConditionData {
public:
    std::strong_ordering operator<=>(const ConditionData&) const noexcept = default;

    std::string condition_str;
};

Errors validate_condition(const ConditionData& data);

} // namespace dnd

#endif // CONDITION_DATA_HPP_
