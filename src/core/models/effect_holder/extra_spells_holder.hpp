#ifndef EXTRA_SPELLS_HOLDER_HPP_
#define EXTRA_SPELLS_HOLDER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_set>

namespace dnd {

/**
 * @brief This class hold any spells a character gains through any other means than his class' spellcasting feature.
 */
class ExtraSpellsHolder {
public:
    /**
     * @brief Returns true if the ExtraSpellsHolder is empty
     * @return "true" if the ExtraSpellsHolder is empty, "false" otherwise
     */
    bool empty() const;

    // cantrips that do not count against the number of cantrips known
    std::unordered_set<std::string> free_cantrips;
    // spells that you can cast without expending a spell slot or material components
    std::unordered_set<std::string> at_will;
    // spells that you can cast once a day (or rather once between two long rests)
    // these spells do not require spell slots
    std::unordered_set<std::string> innate;
    // spells that you can cast once a day (or rather once between two long rests)
    // these spells do require spell slots
    std::unordered_set<std::string> free_once_a_day;
    // spells that are added to your spell list and you know them / you always have them prepared
    // (these spells do not count to the number of spells you know)
    std::unordered_set<std::string> spells_known;
    // spells that are added to your spell list and you know them
    // these spells do count to the number of spells you know
    std::unordered_set<std::string> spells_known_included;
    // spells that are added to your spell list
    std::unordered_set<std::string> added_to_spell_list;
};

inline bool ExtraSpellsHolder::empty() const {
    return free_cantrips.empty() && at_will.empty() && innate.empty() && free_once_a_day.empty() && spells_known.empty()
           && spells_known_included.empty() && added_to_spell_list.empty();
}

} // namespace dnd

#endif // EXTRA_SPELLS_HOLDER_HPP_
