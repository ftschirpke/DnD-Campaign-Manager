#include <dnd_config.hpp>

#include "advanced_content_search.hpp"

#include <memory>
#include <vector>

#include <core/content.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

dnd::AdvancedContentSearch::AdvancedContentSearch(const Content& content) noexcept
    : content(content), filter(ContentPieceFilter()) {}

dnd::ContentFilterVariant& dnd::AdvancedContentSearch::get_filter() { return filter; }
