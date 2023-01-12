#ifndef PREPARATION_SPELLCASTING_HPP_
#define PREPARATION_SPELLCASTING_HPP_

#include "models/spellcasting/spellcasting.hpp"

namespace dnd {

enum PreparationSpellcastingType {
    HALF,
    FULL,
};

class PreparationSpellcasting : public Spellcasting {
public:
    PreparationSpellcasting(
        const std::string& spellcasting_ability, bool ritual_casting, PreparationSpellcastingType type
    );
    const PreparationSpellcastingType type;
};

inline PreparationSpellcasting::PreparationSpellcasting(
    const std::string& spellcasting_ability, bool ritual_casting, PreparationSpellcastingType type
)
    : Spellcasting(spellcasting_ability, ritual_casting), type(type) {}

} // namespace dnd

#endif // PREPARATION_SPELLCASTING_HPP_
