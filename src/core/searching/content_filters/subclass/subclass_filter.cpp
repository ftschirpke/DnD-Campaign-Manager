#include <dnd_config.hpp>

#include "subclass_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

bool SubclassFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && has_spellcasting_filter.is_set();
}

bool SubclassFilter::matches(const Subclass& subclass) const {
    return ContentPieceFilter::matches(subclass) && has_spellcasting_filter.matches(subclass.has_spellcasting());
}

std::vector<const ContentPiece*> SubclassFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, subclass] : content.get_subclasses().get_all()) {
        if (matches(subclass)) {
            matching_content_pieces.emplace_back(&subclass);
        }
    }
    return matching_content_pieces;
}

void SubclassFilter::clear() {
    ContentPieceFilter::clear();
    has_spellcasting_filter.clear();
}

} // namespace dnd
