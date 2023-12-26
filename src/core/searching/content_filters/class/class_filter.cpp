#include <dnd_config.hpp>

#include "class_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/class/class.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::ClassFilter::has_all_filters() const noexcept {
    return ContentPieceFilter::has_all_filters() && has_spellcasting_filter.is_set();
}

bool dnd::ClassFilter::matches(const Class& cls) const noexcept {
    return ContentPieceFilter::matches(cls) && has_spellcasting_filter.matches(cls.has_spellcasting());
}

std::vector<const dnd::ContentPiece*> dnd::ClassFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, cls] : content.get_classes().get_all()) {
        if (matches(cls)) {
            matching_content_pieces.emplace_back(&cls);
        }
    }
    return matching_content_pieces;
}

void dnd::ClassFilter::clear() {
    ContentPieceFilter::clear();
    has_spellcasting_filter.clear();
}
