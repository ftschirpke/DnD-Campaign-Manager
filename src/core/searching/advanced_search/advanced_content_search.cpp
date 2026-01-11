#include <dnd_config.hpp>

#include "advanced_content_search.hpp"

#include <future>
#include <vector>

#include <core/content.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/types.hpp>

namespace dnd {

AdvancedContentSearch::AdvancedContentSearch(const Content& content)
    : content(content), filter(ContentPieceFilter(content)), searching(false), search_future() {}

ContentFilterVariant& AdvancedContentSearch::get_filter() { return filter; }

const std::vector<Id>& AdvancedContentSearch::get_search_results() const { return search_results; }

static std::vector<Id> search(const Content& content, ContentFilterVariant searching_filter) {
    std::vector<Id> search_results = dispatch(searching_filter, const auto& filter, filter.all_matches(content));
    std::sort(search_results.begin(), search_results.end(), [&content](Id lhs, Id rhs) {
        auto lhs_res = content.get(lhs);
        auto rhs_res = content.get(rhs);
        auto lhs_name = dispatch(lhs_res, const auto& piece, piece.get().get_name());
        auto rhs_name = dispatch(rhs_res, const auto& piece, piece.get().get_name());
        return lhs_name < rhs_name;
    });
    return search_results;
}

void AdvancedContentSearch::start_searching() {
    searching = true;
    search_future = std::async(std::launch::async, search, std::ref(content), filter);
}

bool AdvancedContentSearch::search_results_available() {
    if (searching && search_future.wait_for(std::chrono::nanoseconds(1)) == std::future_status::ready) {
        searching = false;
        search_results = search_future.get();
    }
    return !searching;
}

} // namespace dnd
