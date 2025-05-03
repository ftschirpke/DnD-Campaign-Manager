#ifndef SPELLCASTING_FACTORY_HPP_
#define SPELLCASTING_FACTORY_HPP_

#include <dnd_config.hpp>

#include <core/models/spellcasting/spellcasting.hpp>
#include <core/data_result.hpp>

namespace dnd {

FactoryResult<Spellcasting> create_spellcasting(Spellcasting::Data&& data);

} // namespace dnd

#endif // SPELLCASTING_FACTORY_HPP_
