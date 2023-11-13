#ifndef CONTENT_FILTER_HPP_
#define CONTENT_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/content_piece.hpp>

namespace dnd {

class Content;

/**
 * @brief An interface for filtering content
 */
class ContentFilter {
public:
    virtual ~ContentFilter() = default;

    /**
     * @brief Determines whether all filters are set
     * @return "true" if all filters are set, "false" otherwise
     */
    virtual bool has_all_filters() const noexcept = 0;

    /**
     * @brief Get all content pieces that match the filter
     * @param content the content to search through
     * @return a vector of pointers to the content pieces that match the filter
     */
    virtual std::vector<const ContentPiece*> all_matches(const Content& content) const = 0;

    /**
     * @brief Clears all filter settings
     */
    virtual void clear() = 0;
};

} // namespace dnd

#endif // CONTENT_FILTER_HPP_
