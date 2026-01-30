#ifndef SPECIES_FILTER_HPP_
#define SPECIES_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/species/species.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class SpeciesFilter : public ContentPieceFilter {
public:
    explicit SpeciesFilter(const Content& content) noexcept;

    bool has_all_filters() const override final;
    bool matches(const Species& species) const;
    std::vector<Id> all_matches() const override final;
    void clear() override final;
};

} // namespace dnd

#endif // SPECIES_FILTER_HPP_
