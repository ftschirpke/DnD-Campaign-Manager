#ifndef CLASS_VALIDATION_HPP_
#define CLASS_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/class/class.hpp>

namespace dnd {

class Content;

Errors validate_class_nonrecursively_for_content(const Class::Data& data, const Content& content);
Errors validate_class_recursively_for_content(const Class::Data& data, const Content& content);

} // namespace dnd

#endif // CLASS_VALIDATION_HPP_
