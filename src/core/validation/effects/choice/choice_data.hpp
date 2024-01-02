#ifndef CHOICE_VALIDATION_HPP_
#define CHOICE_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/models/effects/choice/choice.hpp>

namespace dnd {

class Content;

Errors validate_choice_for_content(const Choice::Data& data, const Content& content);

} // namespace dnd

#endif // CHOICE_VALIDATION_HPP_
