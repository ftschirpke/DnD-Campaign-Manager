#ifndef SPELLCASTING_HPP_
#define SPELLCASTING_HPP_

#include <dnd_config.hpp>

#include <array>
#include <string>
#include <unordered_map>

#include <core/models/spell.hpp>

namespace dnd {

/**
 * @brief A class representing the spellcasting abilities a class can provide
 */
class Spellcasting {
public:
    /**
     * @brief Constructs a Spellcasting object
     * @param spellcasting_ability the ability used for spellcasting
     * @param ritual_casting "true" if this type of spellcasting allows ritual casting, "false" otherwise
     */
    Spellcasting(const std::string& spellcasting_ability, bool ritual_casting);
    virtual ~Spellcasting() noexcept = default;

    // the spellcasting ability i.e. the ability use for spellcasting
    const std::string ability;
    // says whether this type of spellcasting allows ritual casting
    const bool ritual_casting;
    // pointers to the spells on the spell list mapped to the spell names
    std::unordered_map<std::string, const Spell*> spell_list;
protected:
    // the amount of cantrips you know as a spell caster of this type for class level 1 through 20
    std::array<int, 20> cantrips_known;
    // the amount of spell slots you have as a spell caster of this type
    // for spell slot levels 1 through 9 and class level 1 through 20
    std::array<std::array<int, 20>, 9> spell_slots;
};

inline Spellcasting::Spellcasting(const std::string& spellcasting_ability, bool ritual_casting)
    : ability(spellcasting_ability), ritual_casting(ritual_casting) {}

} // namespace dnd

#endif // SPELLCASTING_HPP_
