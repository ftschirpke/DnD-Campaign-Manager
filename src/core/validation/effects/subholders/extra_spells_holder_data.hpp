#ifndef EXTRA_SPELLS_HOLDER_DATA_HPP_
#define EXTRA_SPELLS_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>
#include <string>

#include <core/errors/errors.hpp>

namespace dnd {

class Content;

class ExtraSpellsHolderData {
public:
    std::strong_ordering operator<=>(const ExtraSpellsHolderData&) const noexcept = default;
    bool empty() const noexcept;

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

Errors validate_extra_spells_holder_for_content(const ExtraSpellsHolderData& data, const Content& content);

} // namespace dnd

#endif // EXTRA_SPELLS_HOLDER_DATA_HPP_
