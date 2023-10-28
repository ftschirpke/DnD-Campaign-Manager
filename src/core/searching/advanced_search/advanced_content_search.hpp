#ifndef ADVANCED_CONTENT_SEARCH_HPP_
#define ADVANCED_CONTENT_SEARCH_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <vector>

#include <core/content.hpp>
#include <core/searching/content_filters/content_filter.hpp>

namespace dnd {

/**
 * @brief A class representing an advanced content search using content filters.
 */
class AdvancedContentSearch {
public:
    /**
     * @brief Initialize the advanced search with the content that should be searched
     * @param content the content to search
     */
    AdvancedContentSearch(const Content& content) noexcept;
    /**
     * @brief Add a content filter to the search
     * @param filter the filter to add
     */
    void add_filter(std::unique_ptr<ContentFilter> filter);
    /**
     * @brief Get all the content filters in the search
     * @return a vector containing points to all the content filters
     */
    std::vector<ContentFilter*> get_filters();
private:
    const Content& content;
    std::vector<std::unique_ptr<ContentFilter>> filters;
};

} // namespace dnd

#endif // ADVANCED_CONTENT_SEARCH_HPP_
