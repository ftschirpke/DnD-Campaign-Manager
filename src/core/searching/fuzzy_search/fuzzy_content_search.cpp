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
        for (const auto& a : content.get_all_##p()) {                                                                  \
            int64_t match_score = fuzzy_match_string(search_query, a.get_name());                                      \
            if (match_score > min_match_score) {                                                                       \
                results.emplace_back(&a, match_score);                                                                 \
            }                                                                                                          \
        }                                                                                                              \
    }
    X_OWNED_CONTENT_PIECES
#undef X

    if (options.search_features) {
        for (const auto& feature : content.get_all_features()) {
            int64_t match_score = fuzzy_match_string(search_query, feature.get().get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&feature.get(), match_score);
            }
        }
        for (const auto& feature : content.get_all_class_features()) {
            int64_t match_score = fuzzy_match_string(search_query, feature.get().get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&feature.get(), match_score);
            }
        }
        for (const auto& feature : content.get_all_subclass_features()) {
            int64_t match_score = fuzzy_match_string(search_query, feature.get().get_name());
            if (match_score > min_match_score) {
                results.emplace_back(&feature.get(), match_score);
            }
        }
    }
    return results;
}

} // namespace dnd
