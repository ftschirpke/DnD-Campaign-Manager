#ifndef DATA_HPP_
#define DATA_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/character/character.hpp>

namespace dnd {

class Content;

Errors validate_character_nonrecursively_for_content(const Character::Data& data, const Content& content);
Errors validate_character_recursively_for_content(const Character::Data& data, const Content& content);

} // namespace dnd

#endif // DATA_HPP_
