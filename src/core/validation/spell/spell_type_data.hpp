#ifndef SPELL_TYPE_VALIDATION_HPP_
#define SPELL_TYPE_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell_type.hpp>

namespace dnd {

Errors validate_spell_type(const SpellType::Data& data);

} // namespace dnd

#endif // SPELL_TYPE_VALIDATION_HPP_
