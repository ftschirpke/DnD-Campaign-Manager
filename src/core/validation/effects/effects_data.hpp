#ifndef EFFECT_HOLDER_VALIDATION_HPP_
#define EFFECT_HOLDER_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects/effects.hpp>

namespace dnd {

class Content;

Errors validate_effects_recursively_for_content(const Effects::Data& data, const Content& content);

} // namespace dnd

#endif // EFFECT_HOLDER_VALIDATION_HPP_
