#ifndef SEARCH_RESULT_HPP_
#define SEARCH_RESULT_HPP_

#include <dnd_config.hpp>

#include <core/models/content_piece.hpp>

namespace dnd {

struct SearchResult {
    const ContentPiece* content_piece_ptr;
    int64_t significance;
};

} // namespace dnd

#endif // SEARCH_RESULT_HPP_
