#include <dnd_config.hpp>

#include "character_class_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::CharacterClassFilter::has_all_filters() const noexcept {
    return ContentPieceFilter::has_all_filters() && has_spellcasting_filter.is_set();
}

bool dnd::CharacterClassFilter::matches(const CharacterClass& character_class) const noexcept {
    return ContentPieceFilter::matches(character_class)
           && has_spellcasting_filter.matches(character_class.has_spellcasting());
}

std::vector<const dnd::ContentPiece*> dnd::CharacterClassFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character_class] : content.get_character_classes().get_all()) {
        if (matches(character_class)) {
            matching_content_pieces.emplace_back(&character_class);
        }
    }
    return matching_content_pieces;
}

void dnd::CharacterClassFilter::clear() {
    ContentPieceFilter::clear();
    has_spellcasting_filter.clear();
}
