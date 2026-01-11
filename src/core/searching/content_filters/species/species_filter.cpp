#include <dnd_config.hpp>

#include "species_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/species/species.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

SpeciesFilter::SpeciesFilter(const Content& content) noexcept : ContentPieceFilter(content) {}

bool SpeciesFilter::has_all_filters() const { return ContentPieceFilter::has_all_filters(); }

bool SpeciesFilter::matches(const Species& species) const { return ContentPieceFilter::matches(species); }

std::vector<Id> SpeciesFilter::all_matches() const {
    std::vector<Id> matching_content_pieces;
    const std::vector<Species>& species = content.get().get_all_species();
    for (size_t i = 0; i < species.size(); ++i) {
        if (matches(species[i])) {
            matching_content_pieces.push_back(Id{.index = i, .type = Type::Species});
        }
    }
    return matching_content_pieces;
}

void SpeciesFilter::clear() { ContentPieceFilter::clear(); }

} // namespace dnd
