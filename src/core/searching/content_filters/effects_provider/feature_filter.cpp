#include <dnd_config.hpp>

#include "feature_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::FeatureFilter::has_all_filters() const noexcept { return ContentPieceFilter::has_all_filters(); }

bool dnd::FeatureFilter::matches(const Feature& feature) const noexcept { return ContentPieceFilter::matches(feature); }

std::vector<const dnd::ContentPiece*> dnd::FeatureFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, feature] : content.get_features().get_all()) {
        if (matches(*feature)) {
            matching_content_pieces.emplace_back(feature);
        }
    }
    return matching_content_pieces;
}

void dnd::FeatureFilter::clear() { ContentPieceFilter::clear(); }
