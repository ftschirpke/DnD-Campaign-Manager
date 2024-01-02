#ifndef IMPORTANT_LEVELS_VALIDATION_HPP_
#define IMPORTANT_LEVELS_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>

#include <core/errors/errors.hpp>
#include <core/models/class/important_levels.hpp>

namespace dnd {

Errors validate_important_levels(const ImportantLevels::Data& data);

} // namespace dnd

#endif // IMPORTANT_LEVELS_VALIDATION_HPP_
