#include <dnd_config.hpp>

#include "choosable_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::ChoosableFilter::has_all_filters() const noexcept {
    return ContentPieceFilter::has_all_filters() && type_filter.is_set() && has_prerequisites_filter.is_set();
}

bool dnd::ChoosableFilter::matches(const Choosable& choosable) const noexcept {
    return ContentPieceFilter::matches(choosable) && type_filter.matches(choosable.get_type())
           && has_prerequisites_filter.matches(!choosable.get_prerequisites().empty());
}

std::vector<const dnd::ContentPiece*> dnd::ChoosableFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, choosable] : content.get_choosables().get_all()) {
        if (matches(choosable)) {
            matching_content_pieces.emplace_back(&choosable);
        }
    }
    return matching_content_pieces;
}

void dnd::ChoosableFilter::clear() {
    ContentPieceFilter::clear();
    type_filter.clear();
    has_prerequisites_filter.clear();
}