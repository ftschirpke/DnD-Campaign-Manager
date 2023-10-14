#include <dnd_config.hpp>

#include "preparation_spellcasting.hpp"

#include <array>
#include <string>
#include <utility>

#include <core/basic_mechanics/abilities.hpp>
#include <core/models/character/ability_scores.hpp>
#include <core/models/spellcasting/spellcasting.hpp>

dnd::PreparationSpellcastingType dnd::PreparationSpellcasting::get_type() const noexcept { return type; }

int dnd::PreparationSpellcasting::get_prepared_spells_amount(AbilityScores ability_scores, int level) const noexcept {
    if (level < 1 || level > 20) {
        return 0;
    }
    int prepared_spells_amount = 0;
    switch (type) {
        case PreparationSpellcastingType::HALF:
            prepared_spells_amount += level / 2;
            break;
        case PreparationSpellcastingType::FULL:
            prepared_spells_amount += level;
            break;
        default:
            return 0;
    }
    prepared_spells_amount += ability_scores.get_modifier(get_ability());
    return prepared_spells_amount;
}

dnd::PreparationSpellcasting::PreparationSpellcasting(
    Ability spellcasting_ability, bool ritual_casting, std::array<int, 20>&& cantrips_known,
    std::array<std::array<int, 20>, 9>&& spell_slots, PreparationSpellcastingType type
) noexcept
    : Spellcasting(spellcasting_ability, ritual_casting, std::move(cantrips_known), std::move(spell_slots)),
      type(type) {}
