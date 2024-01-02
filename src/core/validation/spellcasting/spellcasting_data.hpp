#ifndef SPELLCASTING_VALIDATION_HPP_
#define SPELLCASTING_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <array>
#include <string>

#include <core/errors/errors.hpp>
#include <core/models/spellcasting/spellcasting.hpp>

namespace dnd {

Errors validate_spellcasting(const Spellcasting::Data& data);

} // namespace dnd

#endif // SPELLCASTING_VALIDATION_HPP_
