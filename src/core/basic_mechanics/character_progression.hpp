#ifndef CHARACTER_PROGRESSION_HPP_
#define CHARACTER_PROGRESSION_HPP_

#include <dnd_config.hpp>

#include <expected>

#include <core/errors/runtime_error.hpp>

namespace dnd {

std::expected<int, RuntimeError> xp_for_level(int level);

std::expected<int, RuntimeError> level_for_xp(int xp);

std::expected<int, RuntimeError> proficiency_bonus_for_level(int level);

} // namespace dnd

#endif // CHARACTER_PROGRESSION_HPP_
