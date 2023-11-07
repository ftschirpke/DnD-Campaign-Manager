#ifndef ITEM_FILTER_HPP_
#define ITEM_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/item/item.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/string_filter.hpp>

namespace dnd {

class ItemFilter : public ContentPieceFilter {
public:
    /**
     * @brief Determines whether all filters are set
     * @return "true" if all filters are set, "false" otherwise
     */
    bool has_all_filters() const noexcept override;

    /**
     * @brief Determines whether an item matches the filter
     * @param content the item to check
     * @return "true" if the item matches the filter, "false" otherwise
     */
    bool matches(const Item& item) const noexcept;

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

    StringFilter cosmetic_description_filter;
    BoolFilter attunement_filter;
};

} // namespace dnd

#endif // ITEM_FILTER_HPP_
