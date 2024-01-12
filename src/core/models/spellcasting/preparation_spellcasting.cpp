#include <dnd_config.hpp>

#include "preparation_spellcasting.hpp"

#include <array>
#include <utility>

#include <core/basic_mechanics/abilities.hpp>
#include <core/models/character/ability_scores.hpp>
#include <core/models/spellcasting/spellcasting.hpp>

namespace dnd {

PreparationSpellcastingType PreparationSpellcasting::get_type() const { return type; }

int PreparationSpellcasting::get_prepared_spells_amount(AbilityScores ability_scores, int level) const {
    if (level < 1 || level > 20) {
        return 0;
    }
    int prepared_spells_amount = ability_scores.get_modifier(get_ability());
    switch (type) {
        case PreparationSpellcastingType::HALF:
            prepared_spells_amount += level / 2;
            return prepared_spells_amount;
        case PreparationSpellcastingType::FULL:
            prepared_spells_amount += level;
            return prepared_spells_amount;
    }
    assert(false);
    return 0;
}

PreparationSpellcasting::PreparationSpellcasting(
    Ability spellcasting_ability, bool ritual_casting, std::array<int, 20>&& cantrips_known,
    std::array<std::array<int, 20>, 9>&& spell_slots, PreparationSpellcastingType type
)
    : Spellcasting(spellcasting_ability, ritual_casting, std::move(cantrips_known), std::move(spell_slots)),
      type(type) {}

} // namespace dnd
