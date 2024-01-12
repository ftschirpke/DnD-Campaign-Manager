#include <dnd_config.hpp>

#include "feature_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

bool FeatureFilter::has_all_filters() const { return ContentPieceFilter::has_all_filters(); }

bool FeatureFilter::matches(const Feature& feature) const { return ContentPieceFilter::matches(feature); }

std::vector<const ContentPiece*> FeatureFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, feature] : content.get_features().get_all()) {
        if (matches(feature)) {
            matching_content_pieces.push_back(&feature.get());
        }
    }
    return matching_content_pieces;
}

void FeatureFilter::clear() { ContentPieceFilter::clear(); }

} // namespace dnd
