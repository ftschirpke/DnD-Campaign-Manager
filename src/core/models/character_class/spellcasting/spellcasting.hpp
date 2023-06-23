#ifndef SPELLCASTING_HPP_
#define SPELLCASTING_HPP_

#include <dnd_config.hpp>

#include <array>
#include <string>

#include <core/basic_mechanics/abilities.hpp>

namespace dnd {

/**
 * @brief A class representing the spellcasting abilities a class can provide
 */
class Spellcasting {
public:
    virtual ~Spellcasting() noexcept = default;

    Ability get_ability() const noexcept;
    bool allows_ritual_casting() const noexcept;
    /**
     * @brief Returns the amount of cantrips known for a given class level
     * @param class_level the class level
     * @return the amount of cantrips known for the given class level, 0 if the class level is invalid
     */
    int get_cantrips_known(int class_level) const;
    /**
     * @brief Returns the amount of spell slots for a given spell slot level and class level
     * @param spell_slot_level the spell slot level
     * @param class_level the class level
     * @return the amount of spell slots for the given spell slot level and class level,
     * 0 if either the spell slot level or the class level is invalid
     */
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

} // namespace dnd

#endif // SPELLCASTING_HPP_
