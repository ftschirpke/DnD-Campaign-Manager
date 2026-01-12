#ifndef SEARCH_RESULT_HPP_
#define SEARCH_RESULT_HPP_

#include <dnd_config.hpp>

#include <core/models/content_piece.hpp>
#include <core/types.hpp>

namespace dnd {

class SearchResult {
public:
    SearchResult() noexcept = default;
    SearchResult(Id content_piece_id, int64_t significance) noexcept;

    Id content_piece_id;
    int64_t significance;
};


inline SearchResult::SearchResult(Id content_piece_id, int64_t significance) noexcept
    : content_piece_id(content_piece_id), significance(significance) {}

} // namespace dnd

#endif // SEARCH_RESULT_HPP_
