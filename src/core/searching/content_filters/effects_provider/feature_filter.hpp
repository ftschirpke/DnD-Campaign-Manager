#ifndef FEATURE_FILTER_HPP_
#define FEATURE_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/effects_provider/feature.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

// TODO: Extend this class

class FeatureFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const override;
    bool matches(const Feature& feature) const;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;
};

} // namespace dnd

#endif // FEATURE_FILTER_HPP_
