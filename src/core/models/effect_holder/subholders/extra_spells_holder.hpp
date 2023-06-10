#ifndef EXTRA_SPELLS_HOLDER_HPP_
#define EXTRA_SPELLS_HOLDER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/validation/effect_holder/subholders/extra_spells_holder_data.hpp>

namespace dnd {

class ContentHolder;
class Spell;

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

    const std::vector<const Spell*>& get_free_cantrips() const noexcept;
    const std::vector<const Spell*>& get_at_will() const noexcept;
    const std::vector<const Spell*>& get_innate() const noexcept;
    const std::vector<const Spell*>& get_free_once_a_day() const noexcept;
    const std::vector<const Spell*>& get_spells_known() const noexcept;
    const std::vector<const Spell*>& get_spells_known_included() const noexcept;
    const std::vector<const Spell*>& get_added_to_spell_list() const noexcept;
protected:
private:
    ExtraSpellsHolder(
        std::vector<const Spell*>&& free_cantrips, std::vector<const Spell*>&& at_will,
        std::vector<const Spell*>&& innate, std::vector<const Spell*>&& free_once_a_day,
        std::vector<const Spell*>&& spells_known, std::vector<const Spell*>&& spells_known_included,
        std::vector<const Spell*>&& added_to_spell_list
    ) noexcept;

    std::vector<const Spell*> free_cantrips;
    std::vector<const Spell*> at_will;
    std::vector<const Spell*> innate;
    std::vector<const Spell*> free_once_a_day;
    std::vector<const Spell*> spells_known;
    std::vector<const Spell*> spells_known_included;
    std::vector<const Spell*> added_to_spell_list;
};

} // namespace dnd

#endif // EXTRA_SPELLS_HOLDER_HPP_
