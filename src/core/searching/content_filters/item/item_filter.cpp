#include <dnd_config.hpp>

#include "item_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/item/item.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

ItemFilter::ItemFilter(const Content& content) noexcept : ContentPieceFilter(content) {}

bool ItemFilter::has_all_filters() const { return ContentPieceFilter::has_all_filters() && attunement_filter.is_set(); }

bool ItemFilter::matches(const Item& item) const {
    return ContentPieceFilter::matches(item) && attunement_filter.matches(item.requires_attunement());
}

std::vector<Id> ItemFilter::all_matches() const {
    std::vector<Id> matching_content_pieces;
    const std::vector<Item>& items = content.get().get_all_items();
    for (size_t i = 0; i < items.size(); ++i) {
        if (matches(items[i])) {
            matching_content_pieces.push_back(Id{.index = i, .type = Type::Item});
        }
    }
    return matching_content_pieces;
}

void ItemFilter::clear() {
    ContentPieceFilter::clear();
    attunement_filter.clear();
}

} // namespace dnd
