#ifndef PROGRESSION_VALIDATION_HPP_
#define PROGRESSION_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/character/progression.hpp>

namespace dnd {

Errors validate_progression(const Progression::Data& data);

} // namespace dnd

#endif // PROGRESSION_VALIDATION_HPP_
