#include <dnd_config.hpp>

#include "subclass_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

SubclassFilter::SubclassFilter(const Content& content) noexcept : ContentPieceFilter(content) {}

bool SubclassFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && has_spellcasting_filter.is_set();
}

bool SubclassFilter::matches(const Subclass& subclass) const {
    return ContentPieceFilter::matches(subclass) && has_spellcasting_filter.matches(subclass.has_spellcasting());
}

std::vector<Id> SubclassFilter::all_matches() const {
    std::vector<Id> matching_content_pieces;
    const std::vector<Subclass>& subclasses = content.get().get_all_subclasses();
    for (size_t i = 0; i < subclasses.size(); ++i) {
        if (matches(subclasses[i])) {
            matching_content_pieces.push_back(Id{.index = i, .type = Type::Subclass});
        }
    }
    return matching_content_pieces;
}

void SubclassFilter::clear() {
    ContentPieceFilter::clear();
    has_spellcasting_filter.clear();
}

} // namespace dnd
