#ifndef DICE_VALIDATION_HPP_
#define DICE_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>

namespace dnd {

Errors validate_dice_string(const std::string& str);

} // namespace dnd

#endif // DICE_VALIDATION_HPP_
