#include <dnd_config.hpp>

#include "spellcasting.hpp"

#include <array>
#include <utility>

#include <core/basic_mechanics/abilities.hpp>

namespace dnd {

Ability Spellcasting::get_ability() const { return ability; }

bool Spellcasting::allows_ritual_casting() const { return ritual_casting; }

int Spellcasting::get_cantrips_known(int class_level) const {
    if (class_level < 1 || class_level > 20) {
        return 0;
    }
    return cantrips_known[static_cast<size_t>(class_level) - 1];
}

int Spellcasting::get_spell_slots(int spell_slot_level, int class_level) const {
    if (spell_slot_level < 1 || spell_slot_level > 9 || class_level < 1 || class_level > 20) {
        return 0;
    }
    return spell_slots[static_cast<size_t>(spell_slot_level) - 1][static_cast<size_t>(class_level) - 1];
}

Spellcasting::Spellcasting(
    Ability spellcasting_ability, bool ritual_casting, std::array<int, 20>&& cantrips_known,
    std::array<std::array<int, 20>, 9>&& spell_slots
)
    : ability(spellcasting_ability), ritual_casting(ritual_casting), cantrips_known(std::move(cantrips_known)),
      spell_slots(std::move(spell_slots)) {}

} // namespace dnd
