#ifndef SELECTION_FILTERING_HPP_
#define SELECTION_FILTERING_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <vector>

namespace dnd {

enum class SelectionFilterType {
    IS_IN,
    IS_NOT_IN,
    NONE,
};

/**
 * @brief Determines if a selection matches a filter.
 * @tparam T the type of the selection
 * @param selection the selection to check
 * @param type the type of the filter (determines how to check the selection)
 * @param filter the filter to check against
 * @return "true" if the selection matches the filter, "false" otherwise
 */
template <typename T>
bool matches_selection(const T& selection, SelectionFilterType type, const std::vector<T>& filter) noexcept;

} // namespace dnd

template <typename T>
bool dnd::matches_selection(const T& selection, SelectionFilterType type, const std::vector<T>& filter) noexcept {
    switch (type) {
        case SelectionFilterType::IS_IN:
            return std::find(filter.begin(), filter.end(), selection) != filter.end();
        case SelectionFilterType::IS_NOT_IN:
            return std::find(filter.begin(), filter.end(), selection) == filter.end();
        case SelectionFilterType::NONE:
            return true;
        default:
            return false;
    }
}

#endif // SELECTION_FILTERING_HPP_
