#ifndef PREPARATION_SPELLCASTING_HPP_
#define PREPARATION_SPELLCASTING_HPP_

#include "dnd_config.hpp"

#include <string>

#include "core/models/spellcasting/spellcasting.hpp"

namespace dnd {

/**
 * @brief An enum for the types of preparation spell casters
 */
enum PreparationSpellcastingType {
    HALF,
    FULL,
};

/**
 * @brief A class representing the spellcasting abilities a class relying on preparation spellcasting can provide
 */
class PreparationSpellcasting : public Spellcasting {
public:
    /**
     * @brief Constructs a PreparationSpellcasting object
     * @param spellcasting_ability the ability used for spellcasting
     * @param ritual_casting "true" if this type of spellcasting allows ritual casting, "false" otherwise
     * @param type the type of preparation spellcasting determining how many spell slots and prepared spells you have
     */
    PreparationSpellcasting(
        const std::string& spellcasting_ability, bool ritual_casting, PreparationSpellcastingType type
    );

    // the type of preparation spellcasting determining how many spell slots and prepared spells you have
    const PreparationSpellcastingType type;
};

inline PreparationSpellcasting::PreparationSpellcasting(
    const std::string& spellcasting_ability, bool ritual_casting, PreparationSpellcastingType type
)
    : Spellcasting(spellcasting_ability, ritual_casting), type(type) {}

} // namespace dnd

#endif // PREPARATION_SPELLCASTING_HPP_
