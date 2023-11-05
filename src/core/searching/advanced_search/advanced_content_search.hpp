#ifndef ADVANCED_CONTENT_SEARCH_HPP_
#define ADVANCED_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <variant>

#include <core/content.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>

namespace dnd {

using ContentFilterVariant = std::variant<ContentPieceFilter, SpellFilter>;

/**
 * @brief A class representing an advanced content search using a content filter.
 */
class AdvancedContentSearch {
public:
    /**
     * @brief Initialize the advanced search with the content that should be searched
     * @param content the content to search
     */
    AdvancedContentSearch(const Content& content) noexcept;
    /**
     * @brief Set the search filter
     * @param new_filter the filter to set
     */
    template <typename T>
    void set_filter(T&& new_filter);
    /**
     * @brief Get the search filter
     * @return the search filter
     */
    ContentFilterVariant& get_filter();
private:
    const Content& content;
    ContentFilterVariant filter;
};

} // namespace dnd

template <typename T>
void dnd::AdvancedContentSearch::set_filter(T&& new_filter) {
    filter = std::move(new_filter);
}

#endif // ADVANCED_CONTENT_SEARCH_HPP_
