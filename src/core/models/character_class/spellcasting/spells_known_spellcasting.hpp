#ifndef SPELLS_KNOWN_SPELLCASTING_HPP_
#define SPELLS_KNOWN_SPELLCASTING_HPP_

#include <dnd_config.hpp>

#include <array>

#include <core/basic_mechanics/abilities.hpp>
#include <core/models/character_class/spellcasting/spellcasting.hpp>

namespace dnd {

class SpellsKnownSpellcasting : public Spellcasting {
public:
    SpellsKnownSpellcasting(
        Ability spellcasting_ability, bool ritual_casting, std::array<int, 20>&& cantrips_known,
        std::array<std::array<int, 20>, 9>&& spell_slots, std::array<int, 20>&& spells_known
    ) noexcept;

    /**
     * @brief Returns the amount of spells a character knows for a given level
     * @param level the level to get the amount of spells known for
     * @return the amount of spells a character knows for the given level
     */
    int get_spells_known(int level) const noexcept;
private:
    std::array<int, 20> spells_known;
};

} // namespace dnd

#endif // SPELLS_KNOWN_SPELLCASTING_HPP_
