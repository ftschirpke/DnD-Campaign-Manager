#include <dnd_config.hpp>

#include "character_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/character/character.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::CharacterFilter::has_all_filters() const noexcept {
    return ContentPieceFilter::has_all_filters() && level_filter.is_set() && xp_filter.is_set();
}

bool dnd::CharacterFilter::matches(const Character& character) const noexcept {
    return ContentPieceFilter::matches(character) && level_filter.matches(character.get_progression().get_level())
           && xp_filter.matches(character.get_progression().get_xp());
}

std::vector<const dnd::ContentPiece*> dnd::CharacterFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character] : content.get_characters().get_all()) {
        if (matches(character)) {
            matching_content_pieces.emplace_back(&character);
        }
    }
    return matching_content_pieces;
}

void dnd::CharacterFilter::clear() {
    ContentPieceFilter::clear();
    level_filter.clear();
    xp_filter.clear();
}
