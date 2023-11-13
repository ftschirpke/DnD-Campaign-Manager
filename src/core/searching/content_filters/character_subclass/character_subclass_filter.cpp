#include <dnd_config.hpp>

#include "character_subclass_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::CharacterSubclassFilter::has_all_filters() const noexcept {
    return ContentPieceFilter::has_all_filters() && has_spellcasting_filter.is_set();
}

bool dnd::CharacterSubclassFilter::matches(const CharacterSubclass& character_subclass) const noexcept {
    return ContentPieceFilter::matches(character_subclass)
           && has_spellcasting_filter.matches(character_subclass.has_spellcasting());
}

std::vector<const dnd::ContentPiece*> dnd::CharacterSubclassFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character_subclass] : content.get_character_subclasses().get_all()) {
        if (matches(character_subclass)) {
            matching_content_pieces.emplace_back(&character_subclass);
        }
    }
    return matching_content_pieces;
}

void dnd::CharacterSubclassFilter::clear() {
    ContentPieceFilter::clear();
    has_spellcasting_filter.clear();
}
