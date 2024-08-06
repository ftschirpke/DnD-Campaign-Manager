#ifndef SEARCH_RESULT_HPP_
#define SEARCH_RESULT_HPP_

#include <dnd_config.hpp>

#include <core/models/content_piece.hpp>

namespace dnd {

class SearchResult {
public:
    SearchResult() = default;
    SearchResult(const ContentPiece* content_piece_ptr, int64_t significance);

    const ContentPiece* content_piece_ptr;
    int64_t significance;
};


inline SearchResult::SearchResult(const ContentPiece* content_piece_ptr, int64_t significance)
    : content_piece_ptr(content_piece_ptr), significance(significance) {}

} // namespace dnd

#endif // SEARCH_RESULT_HPP_
