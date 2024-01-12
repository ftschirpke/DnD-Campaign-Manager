#ifndef FILTER_HPP_
#define FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/character/character.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/number_filter.hpp>

namespace dnd {

class CharacterFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const override;
    bool matches(const Character& character) const;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    NumberFilter<int> level_filter;
    NumberFilter<int> xp_filter;
};


} // namespace dnd

#endif // FILTER_HPP_
