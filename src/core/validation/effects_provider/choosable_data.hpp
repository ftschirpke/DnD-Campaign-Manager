#ifndef CHOOSABLE_VALIDATION_HPP_
#define CHOOSABLE_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/choosable.hpp>

namespace dnd {

Errors validate_choosable_nonrecursively(const Choosable::Data& data);
Errors validate_choosable_recursively_for_content(const Choosable::Data& data, const Content& content);

} // namespace dnd

#endif // CHOOSABLE_VALIDATION_HPP_
