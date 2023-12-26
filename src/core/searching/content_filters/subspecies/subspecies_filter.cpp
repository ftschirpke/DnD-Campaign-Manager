#include <dnd_config.hpp>

#include "character_subspecies_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/character_subspecies/character_subspecies.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::CharacterSubspeciesFilter::has_all_filters() const noexcept { return ContentPieceFilter::has_all_filters(); }

bool dnd::CharacterSubspeciesFilter::matches(const CharacterSubspecies& character_subspecies) const noexcept {
    return ContentPieceFilter::matches(character_subspecies);
}

std::vector<const dnd::ContentPiece*> dnd::CharacterSubspeciesFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character_subspecies] : content.get_character_subspecies().get_all()) {
        if (matches(character_subspecies)) {
            matching_content_pieces.emplace_back(&character_subspecies);
        }
    }
    return matching_content_pieces;
}

void dnd::CharacterSubspeciesFilter::clear() { ContentPieceFilter::clear(); }
