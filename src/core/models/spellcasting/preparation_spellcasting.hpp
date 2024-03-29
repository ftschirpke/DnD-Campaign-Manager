#ifndef PREPARATION_SPELLCASTING_HPP_
#define PREPARATION_SPELLCASTING_HPP_

#include <dnd_config.hpp>

#include <core/basic_mechanics/abilities.hpp>
#include <core/models/character/ability_scores.hpp>
#include <core/models/spellcasting/spellcasting.hpp>

namespace dnd {

enum class PreparationSpellcastingType {
    HALF,
    FULL,
};

class PreparationSpellcasting : public Spellcasting {
public:
    PreparationSpellcasting(
        Ability spellcasting_ability, bool ritual_casting, std::array<int, 20>&& cantrips_known,
        std::array<std::array<int, 20>, 9>&& spell_slots, PreparationSpellcastingType type
    );

    PreparationSpellcastingType get_type() const;
    int get_prepared_spells_amount(AbilityScores ability_scores, int level) const;
private:
    PreparationSpellcastingType type;
};

} // namespace dnd

#endif // PREPARATION_SPELLCASTING_HPP_
