#include <dnd_config.hpp>

#include "subspecies_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::SubspeciesFilter::has_all_filters() const noexcept { return ContentPieceFilter::has_all_filters(); }

bool dnd::SubspeciesFilter::matches(const Subspecies& subspecies) const noexcept {
    return ContentPieceFilter::matches(subspecies);
}

std::vector<const dnd::ContentPiece*> dnd::SubspeciesFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, subspecies] : content.get_subspecies().get_all()) {
        if (matches(subspecies)) {
            matching_content_pieces.emplace_back(&subspecies);
        }
    }
    return matching_content_pieces;
}

void dnd::SubspeciesFilter::clear() { ContentPieceFilter::clear(); }
