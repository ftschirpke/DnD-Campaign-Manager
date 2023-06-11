#ifndef CONDITION_FACTORY_HPP_
#define CONDITION_FACTORY_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <string>
#include <string_view>

#include <core/models/effect_holder/condition/condition.hpp>
#include <core/validation/effect_holder/condition/condition_data.hpp>

namespace dnd {

std::unique_ptr<Condition> create_condition(ConditionData&& condition_data);

} // namespace dnd

#endif // CONDITION_FACTORY_HPP_
