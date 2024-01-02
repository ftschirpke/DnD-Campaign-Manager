#ifndef SUBCLASS_VALIDATION_HPP_
#define SUBCLASS_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/subclass/subclass.hpp>

namespace dnd {

class Content;

Errors validate_subclass_nonrecursively_for_content(const Subclass::Data& data, const Content& content);
Errors validate_subclass_recursively_for_content(const Subclass::Data& data, const Content& content);

} // namespace dnd

#endif // SUBCLASS_VALIDATION_HPP_
