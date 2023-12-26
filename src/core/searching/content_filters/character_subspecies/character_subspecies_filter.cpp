#include <dnd_config.hpp>

#include "character_subrace_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::CharacterSubraceFilter::has_all_filters() const noexcept { return ContentPieceFilter::has_all_filters(); }

bool dnd::CharacterSubraceFilter::matches(const CharacterSubrace& character_subrace) const noexcept {
    return ContentPieceFilter::matches(character_subrace);
}

std::vector<const dnd::ContentPiece*> dnd::CharacterSubraceFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character_subrace] : content.get_character_subraces().get_all()) {
        if (matches(character_subrace)) {
            matching_content_pieces.emplace_back(&character_subrace);
        }
    }
    return matching_content_pieces;
}

void dnd::CharacterSubraceFilter::clear() { ContentPieceFilter::clear(); }
