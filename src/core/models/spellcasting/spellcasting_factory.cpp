#include <dnd_config.hpp>

#include "spellcasting_factory.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>

#include <core/basic_mechanics/abilities.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/spellcasting/preparation_spellcasting.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/models/spellcasting/spells_known_spellcasting.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>

namespace dnd {

std::unique_ptr<Spellcasting> create_spellcasting(SpellcastingData&& data) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create spellcasting from invalid data.");
    }

    if (!data.is_spellcaster) {
        return nullptr;
    }

    Ability ability = string_to_ability(data.ability);
    if (data.is_spells_known_type) {
        return std::make_unique<SpellsKnownSpellcasting>(
            ability, data.ritual_casting, std::move(data.cantrips_known), std::move(data.spell_slots),
            std::move(data.spells_known)
        );
    } else if (data.preparation_spellcasting_type == "half") {
        return std::make_unique<PreparationSpellcasting>(
            ability, data.ritual_casting, std::move(data.cantrips_known), std::move(data.spell_slots),
            PreparationSpellcastingType::HALF
        );
    } else if (data.preparation_spellcasting_type == "full") {
        return std::make_unique<PreparationSpellcasting>(
            ability, data.ritual_casting, std::move(data.cantrips_known), std::move(data.spell_slots),
            PreparationSpellcastingType::FULL
        );
    }
    assert(false);
    return nullptr;
}

} // namespace dnd
