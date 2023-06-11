#ifndef EXTRA_SPELLS_HOLDER_HPP_
#define EXTRA_SPELLS_HOLDER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/validation/effect_holder/subholders/extra_spells_holder_data.hpp>

namespace dnd {

class ContentHolder;
class Spell;

/**
 * @brief This class hold any spells a character gains through any other means than his class' spellcasting feature.
 */
class ExtraSpellsHolder {
public:
    /**
     * @brief Constructs an ExtraSpellsHolder from the given data and content
     * @param data the data to construct the ExtraSpellsHolder from
     * @param content the content to use for the spells
     * @return the constructed ExtraSpellsHolder
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static ExtraSpellsHolder create(ExtraSpellsHolderData&& data, const ContentHolder* content);

    /**
     * @brief Returns whether if the ExtraSpellsHolder is empty
     * @return "true" if the ExtraSpellsHolder is empty, "false" otherwise
     */
    bool empty() const;

    /**
     * @brief Returns the cantrips that do not count against the number of cantrips known
     * @return the cantrips that do not count against the number of cantrips known
     */
    const std::vector<const Spell*>& get_free_cantrips() const noexcept;
    /**
     * @brief Returns the spells that you can cast without expending a spell slot or material components
     * @return the spells that you can cast without expending a spell slot or material components
     */
    const std::vector<const Spell*>& get_at_will() const noexcept;
    /**
     * @brief Returns the spells that you can cast once a day (or rather once between two long rests)
     * these spells do not require spell slots
     * @return the spells that you can cast once a day (or rather once between two long rests)
     */
    const std::vector<const Spell*>& get_innate() const noexcept;
    /**
     * @brief Returns the spells that you can cast once a day (or rather once between two long rests)
     * these spells do require spell slots
     * @return the spells that you can cast once a day (or rather once between two long rests)
     */
    const std::vector<const Spell*>& get_free_once_a_day() const noexcept;
    /**
     * @brief Returns the spells that are added to your spell list and you know them / you always have them prepared
     * (these spells do not count to the number of spells you know)
     * @return the spells that are added to your spell list and you know them / you always have them prepared
     */
    const std::vector<const Spell*>& get_spells_known() const noexcept;
    /**
     * @brief Returns the spells that are added to your spell list and you know them
     * these spells do count to the number of spells you know
     * @return the spells that are added to your spell list and you know them
     */
    const std::vector<const Spell*>& get_spells_known_included() const noexcept;
    /**
     * @brief Returns the spells that are added to your spell list
     * @return the spells that are added to your spell list
     */
    const std::vector<const Spell*>& get_added_to_spell_list() const noexcept;
private:
    ExtraSpellsHolder(
        std::vector<const Spell*>&& free_cantrips, std::vector<const Spell*>&& at_will,
        std::vector<const Spell*>&& innate, std::vector<const Spell*>&& free_once_a_day,
        std::vector<const Spell*>&& spells_known, std::vector<const Spell*>&& spells_known_included,
        std::vector<const Spell*>&& added_to_spell_list
    ) noexcept;

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

} // namespace dnd

#endif // EXTRA_SPELLS_HOLDER_HPP_
