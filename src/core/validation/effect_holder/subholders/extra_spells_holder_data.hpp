#ifndef EXTRA_SPELLS_HOLDER_DATA_HPP_
#define EXTRA_SPELLS_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <set>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ExtraSpellsHolderData : public ValidationSubdata {
public:
    ExtraSpellsHolderData(const ValidationData* parent) noexcept;
    int operator<=>(const ExtraSpellsHolderData&) const noexcept = default;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;
    /**
     * @brief Validates the relations of the data to other content pieces with regard to a given content holder
     * @param content the content holder to validate the relations against
     * @return the errors that occured during validation
     */
    virtual Errors validate_relations(const ContentHolder& content) const override;
    /**
     * @brief Checks if the extra spells holder is empty
     * @return "true" if the extra spells holder is empty, "false" otherwise
     */
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

} // namespace dnd

#endif // EXTRA_SPELLS_HOLDER_DATA_HPP_
