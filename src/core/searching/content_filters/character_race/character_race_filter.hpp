#ifndef CHARACTER_RACE_FILTER_HPP_
#define CHARACTER_RACE_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/character_race/character_race.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class CharacterRaceFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const noexcept override;
    bool matches(const CharacterRace& character_race) const noexcept;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    BoolFilter has_subraces_filter;
};

} // namespace dnd

#endif // CHARACTER_RACE_FILTER_HPP_
