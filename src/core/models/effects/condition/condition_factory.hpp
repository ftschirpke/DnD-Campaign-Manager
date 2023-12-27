#ifndef CONDITION_FACTORY_HPP_
#define CONDITION_FACTORY_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <string>
#include <string_view>

#include <core/models/effects/condition/condition.hpp>
#include <core/validation/effects/condition/condition_data.hpp>

namespace dnd {

/**
 * @brief Constructs a condition
 * @param condition_data the data to construct the condition from
 * @return the constructed condition
 * @throws invalid_data if the data is invalid
 */
std::unique_ptr<Condition> create_condition(ConditionData&& condition_data);

} // namespace dnd

#endif // CONDITION_FACTORY_HPP_
