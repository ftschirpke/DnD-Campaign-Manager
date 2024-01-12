#include <dnd_config.hpp>

#include "advanced_content_search.hpp"

#include <future>
#include <variant>
#include <vector>

#include <core/content.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

AdvancedContentSearch::AdvancedContentSearch(const Content& content)
    : content(content), filter(ContentPieceFilter()), searching(false), search_future() {}

ContentFilterVariant& AdvancedContentSearch::get_filter() { return filter; }

const std::vector<const ContentPiece*>& AdvancedContentSearch::get_search_results() const {
    return search_results;
}

static std::vector<const ContentPiece*> search(const Content& content, ContentFilterVariant searching_filter) {
    std::vector<const ContentPiece*> search_results = std::visit(
        [&content](const ContentFilter& filter) { return filter.all_matches(content); }, searching_filter
    );
    std::sort(search_results.begin(), search_results.end(), [](const ContentPiece* lhs, const ContentPiece* rhs) {
        return lhs->get_name() < rhs->get_name();
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
