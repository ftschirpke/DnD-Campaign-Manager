#ifndef SPELLS_KNOWN_SPELLCASTING_HPP_
#define SPELLS_KNOWN_SPELLCASTING_HPP_

#include "dnd_config.hpp"

#include <array>
#include <string>

#include "models/spellcasting/spellcasting.hpp"

namespace dnd {

/**
 * @brief A class representing the spellcasting abilities a class relying on the knowlege of spells can provide
 */
class SpellsKnownSpellcasting : public Spellcasting {
public:
    /**
     * @brief Constructs a SpellsKnownSpellcasting object
     * @param spellcasting_ability the ability used for spellcasting
     * @param ritual_casting "true" if this type of spellcasting allows ritual casting, "false" otherwise
     */
    SpellsKnownSpellcasting(const std::string& spellcasting_ability, bool ritual_casting);
protected:
    // the amount of spells you know as a spell caster of this type for class level 1 through 20
    std::array<int, 20> spells_known;
};

inline SpellsKnownSpellcasting::SpellsKnownSpellcasting(const std::string& spellcasting_ability, bool ritual_casting)
    : Spellcasting(spellcasting_ability, ritual_casting) {}

} // namespace dnd

#endif // SPELLS_KNOWN_SPELLCASTING_HPP_
