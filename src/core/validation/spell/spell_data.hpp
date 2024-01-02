#ifndef SPELL_VALIDATION_HPP_
#define SPELL_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell.hpp>

namespace dnd {

Errors validate_spell_nonrecursively(const Spell::Data& data);
Errors validate_spell_recursively(const Spell::Data& data);

} // namespace dnd

#endif // SPELL_VALIDATION_HPP_
