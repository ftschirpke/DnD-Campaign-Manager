#ifndef CONTENT_FILTER_HPP_
#define CONTENT_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/content_piece.hpp>
#include <core/types.hpp>

namespace dnd {

class Content;

class ContentFilter {
public:
    virtual ~ContentFilter() = default;
    virtual bool has_all_filters() const = 0;
    virtual std::vector<Id> all_matches() const = 0;
    virtual void clear() = 0;
};

} // namespace dnd

#endif // CONTENT_FILTER_HPP_
