#include <dnd_config.hpp>

#include "character_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/character/character.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

CharacterFilter::CharacterFilter(const Content& content) noexcept : ContentPieceFilter(content) {}

bool CharacterFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && level_filter.is_set() && xp_filter.is_set();
}

bool CharacterFilter::matches(const Character& character) const {
    return ContentPieceFilter::matches(character) && level_filter.matches(character.get_progression().get_level())
           && xp_filter.matches(character.get_progression().get_xp());
}

std::vector<Id> CharacterFilter::all_matches() const {
    std::vector<Id> matching_content_pieces;
    const std::vector<Character>& characters = content.get().get_all_characters();
    for (size_t i = 0; i < characters.size(); ++i) {
        if (matches(characters[i])) {
            matching_content_pieces.push_back(Id{.index = i, .type = Type::Character});
        }
    }
    return matching_content_pieces;
}

void CharacterFilter::clear() {
    ContentPieceFilter::clear();
    level_filter.clear();
    xp_filter.clear();
}

} // namespace dnd
