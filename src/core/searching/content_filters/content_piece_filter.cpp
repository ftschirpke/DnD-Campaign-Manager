#include <dnd_config.hpp>

#include "content_piece_filter.hpp"

#include <variant>
#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/string_filter.hpp>

namespace dnd {

ContentPieceFilter::ContentPieceFilter() : name_filter(StringFilter()), is_sourcebook_filter() {}

bool ContentPieceFilter::has_all_filters() const {
    return std::visit([](const auto& filter) { return filter.is_set(); }, name_filter) && is_sourcebook_filter.is_set();
}

bool ContentPieceFilter::matches(const ContentPiece& content_piece) const {
    return std::visit(
        [&content_piece](const auto& filter) { return filter.matches(content_piece.get_name()); }, name_filter
    );
}

std::vector<const ContentPiece*> ContentPieceFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
#define X(C, U, j, a, p, P)                                                                                            \
    for (const C& a : content.get_all_##p()) {                                                                         \
        if (matches(a)) {                                                                                              \
            matching_content_pieces.push_back(&a);                                                                     \
        }                                                                                                              \
    }
    X_CONTENT_PIECES
#undef X
    return matching_content_pieces;
}

void ContentPieceFilter::clear() {
    name_filter = StringFilter();
    is_sourcebook_filter.clear();
}

} // namespace dnd
