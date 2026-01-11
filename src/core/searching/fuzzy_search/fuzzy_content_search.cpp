#include <cstdint>
#include <dnd_config.hpp>

#include "fuzzy_content_search.hpp"

#include <string>
#include <vector>

#include <core/content.hpp>
#include <core/searching/fuzzy_search/fuzzy_string_search.hpp>
#include <core/searching/search_result.hpp>

namespace dnd {

std::vector<SearchResult> fuzzy_search_content(
    const Content& content, const std::string& search_query, const FuzzySearchOptions& options
) {
    DND_MEASURE_FUNCTION();
    std::vector<SearchResult> results;

    int64_t min_match_score = 0;

#define X(C, U, j, a, p, P)                                                                                            \
    if (options.search_##p) {                                                                                          \
        const std::vector<C>& vec_##p = content.get_all_##p();                                                         \
        for (size_t i = 0; i < vec_##p.size(); ++i) {                                                                  \
            const C& a = vec_##p[i];                                                                                   \
            int64_t match_score = fuzzy_match_string(search_query, a.get_name());                                      \
            if (match_score > min_match_score) {                                                                       \
                results.emplace_back(Id{.index = i, .type = Type::C}, match_score);                                    \
            }                                                                                                          \
        }                                                                                                              \
    }

    X_OWNED_CONTENT_PIECES
#undef X

    if (options.search_features) {
#define X(C, U, j, a, p, P)                                                                                            \
    const std::vector<CRef<C>>& vec_##p = content.get_all_##p();                                                       \
    for (size_t i = 0; i < vec_##p.size(); ++i) {                                                                      \
        const C& a = vec_##p[i];                                                                                       \
        int64_t match_score = fuzzy_match_string(search_query, a.get_name());                                          \
        if (match_score > min_match_score) {                                                                           \
            results.emplace_back(Id{.index = i, .type = Type::C}, match_score);                                        \
        }                                                                                                              \
    }

        X_FEATURES
#undef X
    }
    return results;
}

} // namespace dnd
