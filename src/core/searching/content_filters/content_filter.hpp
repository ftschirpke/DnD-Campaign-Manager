#ifndef CONTENT_FILTER_HPP_
#define CONTENT_FILTER_HPP_

#include <dnd_config.hpp>

#include <core/visitors/filters/content_filter_visitor.hpp>

namespace dnd {

/**
 * @brief An interface for filtering content
 */
class ContentFilter {
public:
    virtual ~ContentFilter() = default;
    /**
     * @brief Clears all filter settings
     */
    virtual void clear() = 0;
};

} // namespace dnd

#endif // CONTENT_FILTER_HPP_
