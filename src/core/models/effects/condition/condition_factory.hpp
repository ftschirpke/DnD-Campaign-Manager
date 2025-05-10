#ifndef CONDITION_FACTORY_HPP_
#define CONDITION_FACTORY_HPP_

#include <dnd_config.hpp>

#include <core/data_result.hpp>
#include <core/models/effects/condition/condition.hpp>

namespace dnd {

FactoryResult<Condition> create_condition(Condition::Data&& condition_data);

} // namespace dnd

#endif // CONDITION_FACTORY_HPP_
