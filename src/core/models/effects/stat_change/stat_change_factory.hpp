#ifndef STAT_CHANGE_FACTORY_HPP_
#define STAT_CHANGE_FACTORY_HPP_

#include <dnd_config.hpp>

#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/data_result.hpp>

namespace dnd {

FactoryResult<StatChange> create_stat_change(StatChange::Data&& stat_change_data);

} // namespace dnd

#endif // STAT_CHANGE_FACTORY_HPP_
