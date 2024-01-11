#ifndef SPELLCASTING_HPP_
#define SPELLCASTING_HPP_

#include <dnd_config.hpp>

#include <array>
#include <string>

#include <core/basic_mechanics/abilities.hpp>

namespace dnd {

class Spellcasting {
public:
    struct Data;

    virtual ~Spellcasting() noexcept = default;

    Ability get_ability() const noexcept;
    bool allows_ritual_casting() const noexcept;

    int get_cantrips_known(int class_level) const;
    int get_spell_slots(int spell_slot_level, int class_level) const;
protected:
    Spellcasting(
        Ability spellcasting_ability, bool ritual_casting, std::array<int, 20>&& cantrips_known,
        std::array<std::array<int, 20>, 9>&& spell_slots
    ) noexcept;
private:
    Ability ability;
    bool ritual_casting;
    std::array<int, 20> cantrips_known;
    std::array<std::array<int, 20>, 9> spell_slots;
};

struct Spellcasting::Data {
    explicit Data() noexcept;
    std::strong_ordering operator<=>(const Spellcasting::Data&) const noexcept = default;

    bool is_spellcaster;
    std::string ability;
    bool ritual_casting;
    bool is_spells_known_type;
    std::string preparation_spellcasting_type;
    std::array<int, 20> spells_known;
    std::array<int, 20> cantrips_known;
    std::array<std::array<int, 20>, 9> spell_slots;
};

inline Spellcasting::Data::Data() noexcept {
    spells_known.fill(0);
    cantrips_known.fill(0);
    for (std::array<int, 20>& arr : spell_slots) {
        arr.fill(0);
    }
}


} // namespace dnd

#endif // SPELLCASTING_HPP_
