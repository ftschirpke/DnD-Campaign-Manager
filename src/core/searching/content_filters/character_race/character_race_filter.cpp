#include <dnd_config.hpp>

#include "character_race_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::CharacterRaceFilter::has_all_filters() const noexcept {
    return ContentPieceFilter::has_all_filters() && has_subraces_filter.is_set();
}

bool dnd::CharacterRaceFilter::matches(const CharacterRace& character_race) const noexcept {
    return ContentPieceFilter::matches(character_race) && has_subraces_filter.matches(character_race.has_subraces());
}

std::vector<const dnd::ContentPiece*> dnd::CharacterRaceFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character_race] : content.get_character_races().get_all()) {
        if (matches(character_race)) {
            matching_content_pieces.emplace_back(&character_race);
        }
    }
    return matching_content_pieces;
}

void dnd::CharacterRaceFilter::clear() {
    ContentPieceFilter::clear();
    has_subraces_filter.clear();
}
