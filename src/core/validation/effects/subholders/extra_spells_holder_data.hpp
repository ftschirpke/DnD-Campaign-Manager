#ifndef EXTRA_SPELLS_HOLDER_VALIDATION_HPP_
#define EXTRA_SPELLS_HOLDER_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects/subholders/extra_spells_holder.hpp>

namespace dnd {

class Content;

Errors validate_extra_spells_holder_for_content(const ExtraSpellsHolder::Data& data, const Content& content);

} // namespace dnd

#endif // EXTRA_SPELLS_HOLDER_VALIDATION_HPP_
