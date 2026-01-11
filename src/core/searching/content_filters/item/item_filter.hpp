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
    explicit ItemFilter(const Content& content) noexcept;

    bool has_all_filters() const override;
    bool matches(const Item& item) const;
    std::vector<Id> all_matches(const Content& content) const override;
    void clear() override;

    // StringFilter cosmetic_description_filter; // TODO: replace with a TextFilter
    BoolFilter attunement_filter;
};

} // namespace dnd

#endif // ITEM_FILTER_HPP_
