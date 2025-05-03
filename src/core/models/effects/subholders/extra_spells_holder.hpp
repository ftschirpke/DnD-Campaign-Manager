#ifndef EXTRA_SPELLS_HOLDER_HPP_
#define EXTRA_SPELLS_HOLDER_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>
#include <string>
#include <vector>

#include <core/data_result.hpp>

namespace dnd {

class Content;
class Spell;

/**
 * @brief This class hold any spells a character gains through any other means than his class' spellcasting feature.
 */
class ExtraSpellsHolder {
public:
    struct Data;

    static CreateResult<ExtraSpellsHolder> create_for(Data&& data, const Content& content);

    /**
     * @brief Returns the cantrips that do not count against the number of cantrips known
     * @return the cantrips that do not count against the number of cantrips known
     */
    const std::vector<const Spell*>& get_free_cantrips() const;
    /**
     * @brief Returns the spells that you can cast without expending a spell slot or material components
     * @return the spells that you can cast without expending a spell slot or material components
     */
    const std::vector<const Spell*>& get_at_will() const;
    /**
     * @brief Returns the spells that you can cast once a day (or rather once between two long rests)
     * these spells do not require spell slots
     * @return the spells that you can cast once a day (or rather once between two long rests)
     */
    const std::vector<const Spell*>& get_innate() const;
    /**
     * @brief Returns the spells that you can cast once a day (or rather once between two long rests)
     * these spells do require spell slots
     * @return the spells that you can cast once a day (or rather once between two long rests)
     */
    const std::vector<const Spell*>& get_free_once_a_day() const;
    /**
     * @brief Returns the spells that are added to your spell list and you know them / you always have them prepared
     * (these spells do not count to the number of spells you know)
     * @return the spells that are added to your spell list and you know them / you always have them prepared
     */
    const std::vector<const Spell*>& get_spells_known() const;
    /**
     * @brief Returns the spells that are added to your spell list and you know them
     * these spells do count to the number of spells you know
     * @return the spells that are added to your spell list and you know them
     */
    const std::vector<const Spell*>& get_spells_known_included() const;
    /**
     * @brief Returns the spells that are added to your spell list
     * @return the spells that are added to your spell list
     */
    const std::vector<const Spell*>& get_added_to_spell_list() const;

    bool empty() const;
    void merge(ExtraSpellsHolder&& other);
private:
    ExtraSpellsHolder(
        std::vector<const Spell*>&& free_cantrips, std::vector<const Spell*>&& at_will,
        std::vector<const Spell*>&& innate, std::vector<const Spell*>&& free_once_a_day,
        std::vector<const Spell*>&& spells_known, std::vector<const Spell*>&& spells_known_included,
        std::vector<const Spell*>&& added_to_spell_list
    );

    // cantrips that do not count against the number of cantrips known
    std::vector<const Spell*> free_cantrips;
    // spells that you can cast without expending a spell slot or material components
    std::vector<const Spell*> at_will;
    // spells that you can cast once a day (or rather once between two long rests)
    // these spells do not require spell slots
    std::vector<const Spell*> innate;
    // spells that you can cast once a day (or rather once between two long rests)
    // these spells do require spell slots
    std::vector<const Spell*> free_once_a_day;
    // spells that are added to your spell list and you know them / you always have them prepared
    // (these spells do not count to the number of spells you know)
    std::vector<const Spell*> spells_known;
    // spells that are added to your spell list and you know them
    // these spells do count to the number of spells you know
    std::vector<const Spell*> spells_known_included;
    // spells that are added to your spell list
    std::vector<const Spell*> added_to_spell_list;
};

struct ExtraSpellsHolder::Data {
    std::strong_ordering operator<=>(const Data&) const = default;
    bool empty() const;

    // cantrips that do not count against the number of cantrips known
    std::set<std::string> free_cantrips;
    // spells that you can cast without expending a spell slot or material components
    std::set<std::string> at_will;
    // spells that you can cast once a day (or rather once between two long rests)
    // these spells do not require spell slots
    std::set<std::string> innate;
    // spells that you can cast once a day (or rather once between two long rests)
    // these spells do require spell slots
    std::set<std::string> free_once_a_day;
    // spells that are added to your spell list and you know them / you always have them prepared
    // (these spells do not count to the number of spells you know)
    std::set<std::string> spells_known;
    // spells that are added to your spell list and you know them
    // these spells do count to the number of spells you know
    std::set<std::string> spells_known_included;
    // spells that are added to your spell list
    std::set<std::string> added_to_spell_list;
};

inline bool ExtraSpellsHolder::Data::empty() const {
    return free_cantrips.empty() && at_will.empty() && innate.empty() && free_once_a_day.empty() && spells_known.empty()
           && spells_known_included.empty() && added_to_spell_list.empty();
}

} // namespace dnd

#endif // EXTRA_SPELLS_HOLDER_HPP_
