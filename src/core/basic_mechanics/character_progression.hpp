#ifndef CHARACTER_PROGRESSION_HPP_
#define CHARACTER_PROGRESSION_HPP_

#include <dnd_config.hpp>

#include <tl/expected.hpp>

#include <core/errors/runtime_error.hpp>

namespace dnd {

tl::expected<int, RuntimeError> xp_for_level(int level);

tl::expected<int, RuntimeError> level_for_xp(int xp);

tl::expected<int, RuntimeError> proficiency_bonus_for_level(int level);

} // namespace dnd

#endif // CHARACTER_PROGRESSION_HPP_
