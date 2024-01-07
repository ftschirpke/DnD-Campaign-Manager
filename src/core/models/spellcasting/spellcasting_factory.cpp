#include <dnd_config.hpp>

#include "spellcasting_factory.hpp"

#include <memory>
#include <string>
#include <utility>

#include <core/basic_mechanics/abilities.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/spellcasting/preparation_spellcasting.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/models/spellcasting/spells_known_spellcasting.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>

namespace dnd {

FactoryResult<Spellcasting> create_spellcasting(Spellcasting::Data&& data) {
    Errors errors = validate_spellcasting(data);
    if (!errors.ok()) {
        return InvalidFactory<Spellcasting>(std::move(data), std::move(errors));
    }

    if (!data.is_spellcaster) {
        return ValidFactory<Spellcasting>(nullptr);
    }

    Ability ability = string_to_ability(data.ability);
    std::unique_ptr<Spellcasting> spellcasting;
    if (data.is_spells_known_type) {
        spellcasting = std::make_unique<SpellsKnownSpellcasting>(
            ability, data.ritual_casting, std::move(data.cantrips_known), std::move(data.spell_slots),
            std::move(data.spells_known)
        );
    } else if (data.preparation_spellcasting_type == "half") {
        spellcasting = std::make_unique<PreparationSpellcasting>(
            ability, data.ritual_casting, std::move(data.cantrips_known), std::move(data.spell_slots),
            PreparationSpellcastingType::HALF
        );
    } else if (data.preparation_spellcasting_type == "full") {
        spellcasting = std::make_unique<PreparationSpellcasting>(
            ability, data.ritual_casting, std::move(data.cantrips_known), std::move(data.spell_slots),
            PreparationSpellcastingType::FULL
        );
    } else {
        Errors unreachable_error(RuntimeError(RuntimeError::Code::UNREACHABLE, "Invalid spellcasting type"));
        return InvalidFactory<Spellcasting>(std::move(data), std::move(unreachable_error));
    }
    return ValidFactory<Spellcasting>(std::move(spellcasting));
}

} // namespace dnd
