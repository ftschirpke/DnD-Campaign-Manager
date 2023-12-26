#include <dnd_config.hpp>

#include "character_species_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/character_species/character_species.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::CharacterSpeciesFilter::has_all_filters() const noexcept {
    return ContentPieceFilter::has_all_filters() && has_subspecies_filter.is_set();
}

bool dnd::CharacterSpeciesFilter::matches(const CharacterSpecies& character_species) const noexcept {
    return ContentPieceFilter::matches(character_species) && has_subspecies_filter.matches(character_species.has_subspecies());
}

std::vector<const dnd::ContentPiece*> dnd::CharacterSpeciesFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character_species] : content.get_character_species().get_all()) {
        if (matches(character_species)) {
            matching_content_pieces.emplace_back(&character_species);
        }
    }
    return matching_content_pieces;
}

void dnd::CharacterSpeciesFilter::clear() {
    ContentPieceFilter::clear();
    has_subspecies_filter.clear();
}
