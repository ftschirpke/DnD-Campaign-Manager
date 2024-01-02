#ifndef SPELLCASTING_FACTORY_HPP_
#define SPELLCASTING_FACTORY_HPP_

#include <dnd_config.hpp>

#include <memory>

#include <core/models/spellcasting/preparation_spellcasting.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/models/spellcasting/spells_known_spellcasting.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>

namespace dnd {

FactoryResult<Spellcasting> create_spellcasting(Spellcasting::Data&& data);

} // namespace dnd

#endif // SPELLCASTING_FACTORY_HPP_
