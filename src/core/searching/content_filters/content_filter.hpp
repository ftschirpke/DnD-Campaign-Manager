#ifndef CONTENT_FILTER_HPP_
#define CONTENT_FILTER_HPP_

#include <dnd_config.hpp>

#include <set>
#include <string>
#include <vector>

#include <core/visitors/filters/content_filter_visitor.hpp>

namespace dnd {

enum class StringFilterType {
    EQUAL,
    NOT_EQUAL,
    CONTAINS,
    NOT_CONTAINS,
    STARTS_WITH,
    NOT_STARTS_WITH,
    ENDS_WITH,
    NOT_ENDS_WITH,
    NONE,
};

enum class NumberFilterType {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    NONE,
};

enum class BoolFilterType {
    IS_TRUE,
    IS_FALSE,
    NONE,
};

enum class SelectionFilterType {
    IS_IN,
    IS_NOT_IN,
    NONE,
};

class ContentFilter {
public:
    virtual ~ContentFilter() = default;
    /**
     * @brief Accepts a visitor to perform an operation on the filter.
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentFilterVisitor& visitor) = 0;
protected:
    static bool matches_string(const std::string& str, StringFilterType type, const std::string& filter) noexcept;
    static bool matches_int(int number, NumberFilterType type, int filter) noexcept;
    static bool matches_float(float number, NumberFilterType type, float filter) noexcept;
    static bool matches_bool(bool boolean, BoolFilterType type) noexcept;
    template <typename T>
    static bool matches_selection(const T& selection, SelectionFilterType type, const std::set<T>& filter) noexcept;
};

template <typename T>
bool ContentFilter::matches_selection(
    const T& selection, SelectionFilterType type, const std::set<T>& filter
) noexcept {
    switch (type) {
        case SelectionFilterType::IS_IN:
            return filter.find(selection) != filter.end();
        case SelectionFilterType::IS_NOT_IN:
            return filter.find(selection) == filter.end();
        default:
            return true;
    }
}

} // namespace dnd

#endif // CONTENT_FILTER_HPP_
