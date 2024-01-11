#ifndef SPELL_COMPONENTS_VALIDATION_HPP_
#define SPELL_COMPONENTS_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell_components.hpp>

namespace dnd {

Errors validate_spell_components(const SpellComponents::Data& data);

} // namespace dnd

#endif // SPELL_COMPONENTS_VALIDATION_HPP_
