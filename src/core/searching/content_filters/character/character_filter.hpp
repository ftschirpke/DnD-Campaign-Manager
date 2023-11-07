#ifndef CHARACTER_FILTER_HPP_
#define CHARACTER_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/character/character.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/number_filter.hpp>

namespace dnd {

class CharacterFilter : public ContentPieceFilter {
public:
    /**
     * @brief Determines whether all filters are set
     * @return "true" if all filters are set, "false" otherwise
     */
    bool has_all_filters() const noexcept override;

    /**
     * @brief Determines whether a character matches the filter
     * @param content the character to check
     * @return "true" if the character matches the filter, "false" otherwise
     */
    bool matches(const Character& character) const noexcept;

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

    NumberFilter<int> level_filter;
    NumberFilter<int> xp_filter;
};


} // namespace dnd

#endif // CHARACTER_FILTER_HPP_
