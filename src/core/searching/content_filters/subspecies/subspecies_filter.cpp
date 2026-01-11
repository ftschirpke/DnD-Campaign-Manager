#include <dnd_config.hpp>

#include "subspecies_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

SubspeciesFilter::SubspeciesFilter(const Content& content) noexcept : ContentPieceFilter(content) {}

bool SubspeciesFilter::has_all_filters() const { return ContentPieceFilter::has_all_filters(); }

bool SubspeciesFilter::matches(const Subspecies& subspecies) const { return ContentPieceFilter::matches(subspecies); }

std::vector<Id> SubspeciesFilter::all_matches() const {
    std::vector<Id> matching_content_pieces;
    const std::vector<Subspecies>& subspecies = content.get().get_all_subspecies();
    for (size_t i = 0; i < subspecies.size(); ++i) {
        if (matches(subspecies[i])) {
            matching_content_pieces.push_back(Id{.index = i, .type = Type::Subspecies});
        }
    }
    return matching_content_pieces;
}

void SubspeciesFilter::clear() { ContentPieceFilter::clear(); }

} // namespace dnd
