#include <dnd_config.hpp>

#include "species_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/species/species.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

bool SpeciesFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && has_subspecies_filter.is_set();
}

bool SpeciesFilter::matches(const Species& species) const {
    return ContentPieceFilter::matches(species) && has_subspecies_filter.matches(species.has_subspecies());
}

std::vector<const ContentPiece*> SpeciesFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, species] : content.get_species().get_all()) {
        if (matches(species)) {
            matching_content_pieces.emplace_back(&species);
        }
    }
    return matching_content_pieces;
}

void SpeciesFilter::clear() {
    ContentPieceFilter::clear();
    has_subspecies_filter.clear();
}

} // namespace dnd
