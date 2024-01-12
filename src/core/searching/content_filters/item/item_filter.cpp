#include <dnd_config.hpp>

#include "item_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/item/item.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

bool ItemFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && cosmetic_description_filter.is_set() && attunement_filter.is_set();
}

bool ItemFilter::matches(const Item& item) const {
    return ContentPieceFilter::matches(item) && cosmetic_description_filter.matches(item.get_cosmetic_description())
           && attunement_filter.matches(item.requires_attunement());
}

std::vector<const ContentPiece*> ItemFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, item] : content.get_items().get_all()) {
        if (matches(item)) {
            matching_content_pieces.push_back(&item);
        }
    }
    return matching_content_pieces;
}

void ItemFilter::clear() {
    ContentPieceFilter::clear();
    cosmetic_description_filter.clear();
    attunement_filter.clear();
}

} // namespace dnd
