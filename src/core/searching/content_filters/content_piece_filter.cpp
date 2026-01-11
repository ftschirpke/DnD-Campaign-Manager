#include <dnd_config.hpp>

#include "content_piece_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/string_filter.hpp>
#include <core/types.hpp>

namespace dnd {

ContentPieceFilter::ContentPieceFilter(const Content& content) noexcept
    : name_filter(StringFilter()), is_sourcebook_filter(), content(std::cref(content)) {}

bool ContentPieceFilter::has_all_filters() const {
    return is_sourcebook_filter.is_set() && dispatch(name_filter, const auto& filter, filter.is_set());
}

bool ContentPieceFilter::matches(const ContentPiece& content_piece) const {
    return dispatch(name_filter, const auto& filter, filter.matches(content_piece.get_name()));
}

std::vector<Id> ContentPieceFilter::all_matches(const Content& content) const {
    std::vector<Id> matching_content_pieces;
#define X(C, U, j, a, p, P)                                                                                            \
    const auto& vec_##p = content.get_all_##p();                                                                       \
    for (size_t i = 0; i < vec_##p.size(); ++i) {                                                                      \
        const C& a = vec_##p[i];                                                                                       \
        if (matches(a)) {                                                                                              \
            matching_content_pieces.emplace_back(Id{.index = i, .type = Type::C});                                     \
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
