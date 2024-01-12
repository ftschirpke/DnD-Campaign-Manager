#include <dnd_config.hpp>

#include "class_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/class/class.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

bool ClassFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && has_spellcasting_filter.is_set();
}

bool ClassFilter::matches(const Class& cls) const {
    return ContentPieceFilter::matches(cls) && has_spellcasting_filter.matches(cls.has_spellcasting());
}

std::vector<const ContentPiece*> ClassFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, cls] : content.get_classes().get_all()) {
        if (matches(cls)) {
            matching_content_pieces.push_back(&cls);
        }
    }
    return matching_content_pieces;
}

void ClassFilter::clear() {
    ContentPieceFilter::clear();
    has_spellcasting_filter.clear();
}

} // namespace dnd
