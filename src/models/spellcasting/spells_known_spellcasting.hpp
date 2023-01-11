#ifndef SPELLS_KNOWN_SPELLCASTING_HPP_
#define SPELLS_KNOWN_SPELLCASTING_HPP_

#include <array>

#include "models/spellcasting/spellcasting.hpp"

namespace dnd {

class SpellsKnownSpellcasting : public Spellcasting {
public:
    SpellsKnownSpellcasting(const std::string& spellcasting_ability, bool ritual_casting);
protected:
    std::array<int, 20> spells_known;
};

inline SpellsKnownSpellcasting::SpellsKnownSpellcasting(const std::string& spellcasting_ability, bool ritual_casting)
    : Spellcasting(spellcasting_ability, ritual_casting) {}

} // namespace dnd

#endif // SPELLS_KNOWN_SPELLCASTING_HPP_
