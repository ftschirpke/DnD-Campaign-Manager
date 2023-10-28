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
     * @brief Accepts a visitor to perform an operation on the filter.
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentFilterVisitor& visitor) = 0;
};

} // namespace dnd

#endif // CONTENT_FILTER_HPP_
