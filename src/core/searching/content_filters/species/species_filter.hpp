#ifndef CHARACTER_RACE_FILTER_HPP_
#define CHARACTER_RACE_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/character_species/character_species.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class CharacterSpeciesFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const noexcept override;
    bool matches(const CharacterSpecies& character_species) const noexcept;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    BoolFilter has_subspecies_filter;
};

} // namespace dnd

#endif // CHARACTER_RACE_FILTER_HPP_
