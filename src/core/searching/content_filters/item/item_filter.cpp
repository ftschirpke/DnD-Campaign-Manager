#include <dnd_config.hpp>

#include "item_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/item/item.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::ItemFilter::has_all_filters() const noexcept {
    return ContentPieceFilter::has_all_filters() && cosmetic_description_filter.is_set() && attunement_filter.is_set();
}

bool dnd::ItemFilter::matches(const Item& item) const noexcept {
    return ContentPieceFilter::matches(item) && cosmetic_description_filter.matches(item.get_cosmetic_description())
           && attunement_filter.matches(item.requires_attunement());
}

std::vector<const dnd::ContentPiece*> dnd::ItemFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, item] : content.get_items().get_all()) {
        if (matches(item)) {
            matching_content_pieces.emplace_back(&item);
        }
    }
    return matching_content_pieces;
}

void dnd::ItemFilter::clear() {
    ContentPieceFilter::clear();
    cosmetic_description_filter.clear();
    attunement_filter.clear();
}
