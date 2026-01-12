#include <dnd_config.hpp>

#include "class_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/class/class.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

ClassFilter::ClassFilter(const Content& content) noexcept : ContentPieceFilter(content) {}

bool ClassFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && has_spellcasting_filter.is_set();
}

bool ClassFilter::matches(const Class& cls) const {
    return ContentPieceFilter::matches(cls) && has_spellcasting_filter.matches(cls.has_spellcasting());
}

std::vector<Id> ClassFilter::all_matches() const {
    std::vector<Id> matching_content_pieces;
    const std::vector<Class>& classes = content.get().get_all_classes();
    for (size_t i = 0; i < classes.size(); ++i) {
        if (matches(classes[i])) {
            matching_content_pieces.push_back(Id{.index = i, .type = Type::Class});
        }
    }
    return matching_content_pieces;
}

void ClassFilter::clear() {
    ContentPieceFilter::clear();
    has_spellcasting_filter.clear();
}

} // namespace dnd
