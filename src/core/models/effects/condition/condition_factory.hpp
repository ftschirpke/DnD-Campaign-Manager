#ifndef CONDITION_FACTORY_HPP_
#define CONDITION_FACTORY_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <string>
#include <string_view>

#include <core/models/effects/condition/condition.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/effects/condition/condition_data.hpp>

namespace dnd {

FactoryResult<Condition> create_condition(Condition::Data&& condition_data);

} // namespace dnd

#endif // CONDITION_FACTORY_HPP_
