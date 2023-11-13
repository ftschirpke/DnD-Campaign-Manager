#include <dnd_config.hpp>

#include "advanced_content_search.hpp"

#include <future>
#include <variant>
#include <vector>

#include <core/content.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

dnd::AdvancedContentSearch::AdvancedContentSearch(const Content& content) noexcept
    : content(content), filter(ContentPieceFilter()), searching(false), search_future() {}

dnd::ContentFilterVariant& dnd::AdvancedContentSearch::get_filter() { return filter; }

const std::vector<const dnd::ContentPiece*>& dnd::AdvancedContentSearch::get_search_results() const noexcept {
    return search_results;
}

static std::vector<const dnd::ContentPiece*> search(
    const dnd::Content& content, dnd::ContentFilterVariant searching_filter
) {
    std::vector<const dnd::ContentPiece*> search_results = std::visit(
        [&content](const dnd::ContentFilter& filter) { return filter.all_matches(content); }, searching_filter
    );
    std::sort(
        search_results.begin(), search_results.end(),
        [](const dnd::ContentPiece* lhs, const dnd::ContentPiece* rhs) { return lhs->get_name() < rhs->get_name(); }
    );
    return search_results;
}

void dnd::AdvancedContentSearch::start_searching() {
    searching = true;
    search_future = std::async(std::launch::async, search, std::ref(content), filter);
}

bool dnd::AdvancedContentSearch::is_searching() {
    if (searching && search_future.wait_for(std::chrono::microseconds(1)) == std::future_status::ready) {
        searching = false;
        search_results = search_future.get();
    }
    return searching;
}
