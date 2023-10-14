#ifndef SPELLCASTING_FACTORY_HPP_
#define SPELLCASTING_FACTORY_HPP_

#include <dnd_config.hpp>

#include <memory>

#include <core/models/spellcasting/preparation_spellcasting.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/models/spellcasting/spells_known_spellcasting.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>

namespace dnd {

/**
 * @brief Constructs a spellcasting object from the given data
 * @param data the data to construct the spellcasting object from
 * @return the constructed spellcasting object
 * @throws dnd::invalid_data if the data is invalid
 */
std::unique_ptr<Spellcasting> create_spellcasting(SpellcastingData&& data);

} // namespace dnd

#endif // SPELLCASTING_FACTORY_HPP_
