#include <dnd_config.hpp>

#include "advanced_content_search.hpp"

#include <memory>
#include <vector>

#include <core/content.hpp>
#include <core/searching/content_filters/content_filter.hpp>

dnd::AdvancedContentSearch::AdvancedContentSearch(const Content& content) noexcept : content(content) {}

void dnd::AdvancedContentSearch::add_filter(std::unique_ptr<ContentFilter> filter) {
    filters.emplace_back(std::move(filter));
}

std::vector<dnd::ContentFilter*> dnd::AdvancedContentSearch::get_filters() {
    std::vector<ContentFilter*> filter_ptrs;
    for (auto& filter : filters) {
        filter_ptrs.emplace_back(filter.get());
    }
    return filter_ptrs;
}
