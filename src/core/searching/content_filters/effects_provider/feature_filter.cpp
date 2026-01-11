#include <dnd_config.hpp>

#include "feature_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

FeatureFilter::FeatureFilter(const Content& content) noexcept : ContentPieceFilter(content) {}

bool FeatureFilter::has_all_filters() const { return ContentPieceFilter::has_all_filters(); }

bool FeatureFilter::matches(const Feature& feature) const { return ContentPieceFilter::matches(feature); }

std::vector<Id> FeatureFilter::all_matches() const {
    std::vector<Id> matching_content_pieces;
    const std::vector<CRef<Feature>>& features = content.get().get_all_features();
    for (size_t i = 0; i < features.size(); ++i) {
        if (matches(features[i])) {
            matching_content_pieces.emplace_back(Id{.index = i, .type = Type::Feature});
        }
    }
    const std::vector<CRef<ClassFeature>>& class_features = content.get().get_all_class_features();
    for (size_t i = 0; i < class_features.size(); ++i) {
        if (matches(class_features[i])) {
            matching_content_pieces.emplace_back(Id{.index = i, .type = Type::ClassFeature});
        }
    }
    const std::vector<CRef<SubclassFeature>>& subclass_features = content.get().get_all_subclass_features();
    for (size_t i = 0; i < subclass_features.size(); ++i) {
        if (matches(subclass_features[i])) {
            matching_content_pieces.emplace_back(Id{.index = i, .type = Type::SubclassFeature});
        }
    }
    return matching_content_pieces;
}

void FeatureFilter::clear() { ContentPieceFilter::clear(); }

} // namespace dnd
