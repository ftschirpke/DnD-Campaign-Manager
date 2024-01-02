#ifndef ACTION_HOLDER_VALIDATION_HPP_
#define ACTION_HOLDER_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <string>

#include <core/errors/errors.hpp>
#include <core/models/effects/subholders/action_holder.hpp>

namespace dnd {

Errors validate_actions_holder(const ActionHolder::Data& data);

} // namespace dnd

#endif // ACTION_HOLDER_VALIDATION_HPP_
