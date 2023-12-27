#ifndef STAT_CHANGE_FACTORY_HPP_
#define STAT_CHANGE_FACTORY_HPP_

#include <dnd_config.hpp>

#include <memory>

#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/validation/effects/stat_change/stat_change_data.hpp>

namespace dnd {

/**
 * @brief Constructs a stat change
 * @param stat_change_data the data to construct the stat change from
 * @return the constructed stat change
 * @throw invalid_data if the data is invalid
 */
std::unique_ptr<StatChange> create_stat_change(StatChangeData&& stat_change_data);

} // namespace dnd

#endif // STAT_CHANGE_FACTORY_HPP_
