#ifndef SPELLCASTING_HPP_
#define SPELLCASTING_HPP_

#include "dnd_config.hpp"

#include <array>
#include <string>
#include <unordered_map>

#include "models/spell.hpp"

namespace dnd {

class Spellcasting {
public:
    Spellcasting(const std::string& spellcasting_ability, bool ritual_casting);
    virtual ~Spellcasting() noexcept = default;
    const std::string ability;
    const bool ritual_casting;
    std::unordered_map<std::string, const Spell*> spell_list;
protected:
    std::array<int, 20> cantrips_known;
    std::array<std::array<int, 20>, 9> spell_slots;
};

inline Spellcasting::Spellcasting(const std::string& spellcasting_ability, bool ritual_casting)
    : ability(spellcasting_ability), ritual_casting(ritual_casting) {}

} // namespace dnd

#endif // SPELLCASTING_HPP_
