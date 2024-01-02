#ifndef EFFECT_VALIDATION_HPP_
#define EFFECT_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

Errors validate_stat_change(const StatChange::Data& data);

} // namespace dnd

#endif // EFFECT_VALIDATION_HPP_
