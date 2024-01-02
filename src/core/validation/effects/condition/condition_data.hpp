#ifndef CONDITION_VALIDATION_HPP_
#define CONDITION_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/models/effects/condition/condition.hpp>

namespace dnd {

Errors validate_condition(const Condition::Data& data);

} // namespace dnd

#endif // CONDITION_VALIDATION_HPP_
