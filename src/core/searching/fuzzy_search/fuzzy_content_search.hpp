#ifndef FUZZY_CONTENT_SEARCH_HPP_
#define FUZZY_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/searching/search_result.hpp>
#include <x/content_pieces.hpp>

namespace dnd {

class Content;

constexpr uint32_t FUZZY_SEARCH_MINIMUM_QUERY_LENGTH = 1;

struct FuzzySearchOptions {
#define X(C, U, j, a, p, P) bool search_##p;
    X_OWNED_CONTENT_PIECES
#undef X
    bool search_features;

    std::strong_ordering operator<=>(const FuzzySearchOptions&) const = default;

    void set_all(bool value) {
#define X(C, U, j, a, p, P) search_##p = value;
        X_OWNED_CONTENT_PIECES
#undef X
        search_features = value;
    }
};

std::vector<SearchResult> fuzzy_search_content(
    const Content& content, const std::string& search_query, const FuzzySearchOptions& options
);


} // namespace dnd

#endif // FUZZY_CONTENT_SEARCH_HPP_
