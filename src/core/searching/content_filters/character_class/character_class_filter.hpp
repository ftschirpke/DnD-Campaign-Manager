#ifndef CHARACTER_CLASS_FILTER_HPP_
#define CHARACTER_CLASS_FILTER_HPP_

#include <dnd_config.hpp>

#include <core/models/character_class/character_class.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class CharacterClassFilter : public ContentPieceFilter {
public:
    /**
     * @brief Determines whether all filters are set
     * @return "true" if all filters are set, "false" otherwise
     */
    bool has_all_filters() const noexcept override;

    /**
     * @brief Determines whether a character class matches the filter
     * @param content the character class to check
     * @return "true" if the character class matches the filter, "false" otherwise
     */
    bool matches(const CharacterClass& character_class) const noexcept;

    /**
     * @brief Get all content pieces that match the filter
     * @param content the content to search through
     * @return a vector of pointers to the content pieces that match the filter
     */
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;

    /**
     * @brief Clears all filter settings
     */
    void clear() override;

    BoolFilter has_spellcasting_filter;
};

} // namespace dnd

#endif // CHARACTER_CLASS_FILTER_HPP_
