#include <dnd_config.hpp>

#include "spells_known_spellcasting.hpp"

#include <array>
#include <utility>

#include <core/basic_mechanics/abilities.hpp>
#include <core/models/character_class/spellcasting/spellcasting.hpp>

int dnd::SpellsKnownSpellcasting::get_spells_known(int level) const noexcept {
    if (level < 1 || level > 20) {
        return 0;
    }
    return spells_known[static_cast<size_t>(level) - 1];
}

dnd::SpellsKnownSpellcasting::SpellsKnownSpellcasting(
    Ability spellcasting_ability, bool ritual_casting, std::array<int, 20>&& cantrips_known,
    std::array<std::array<int, 20>, 9>&& spell_slots, std::array<int, 20>&& spells_known
) noexcept
    : Spellcasting(spellcasting_ability, ritual_casting, std::move(cantrips_known), std::move(spell_slots)),
      spells_known(std::move(spells_known)) {}
