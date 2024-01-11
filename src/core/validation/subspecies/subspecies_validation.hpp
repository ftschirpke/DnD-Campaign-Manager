#ifndef SUBSPECIES_VALIDATION_HPP_
#define SUBSPECIES_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/subspecies/subspecies.hpp>

namespace dnd {

Errors validate_subspecies_nonrecursively_for_content(const Subspecies::Data& data, const Content& content);
Errors validate_subspecies_recursively_for_content(const Subspecies::Data& data, const Content& content);

} // namespace dnd

#endif // SUBSPECIES_VALIDATION_HPP_
