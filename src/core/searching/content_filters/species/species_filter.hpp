#ifndef SPECIES_FILTER_HPP_
#define SPECIES_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/species/species.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class SpeciesFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const override;
    bool matches(const Species& species) const;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    BoolFilter has_subspecies_filter;
};

} // namespace dnd

#endif // SPECIES_FILTER_HPP_
