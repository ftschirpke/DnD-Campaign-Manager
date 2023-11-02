#ifndef SELECTION_FILTERING_HPP_
#define SELECTION_FILTERING_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <utility>
#include <vector>

namespace dnd {

enum class SelectionFilterType {
    IS_IN,
    IS_NOT_IN,
    NONE,
};

/**
 * @brief A class representing a filter for selections.
 * @tparam T the type of the selection
 */
template <typename T>
class SelectionFilter {
public:
    SelectionFilter() noexcept;

    /**
     * @brief Get the type of the filter.
     * @return the type of the filter
     */
    SelectionFilterType get_type() const noexcept;
    /**
     * @brief Set the type of the filter.
     * @param type the type of the filter
     */
    void set_type(SelectionFilterType type) noexcept;
    /**
     * @brief Set the values of the filter.
     * @param values the values of the filter
     */
    void set_values(const std::vector<T>& values) noexcept;
    /**
     * @brief Set the values of the filter.
     * @param values the values of the filter
     */
    void set_values(std::vector<T>&& values) noexcept;
    /**
     * @brief Set the filter.
     * @param type the type of the filter
     * @param values the values of the filter
     */
    void set(SelectionFilterType type, const std::vector<T>& values) noexcept;
    /**
     * @brief Set the filter.
     * @param type the type of the filter
     * @param values the values of the filter
     */
    void set(SelectionFilterType type, std::vector<T>&& values) noexcept;
    /**
     * @brief Clears the filter.
     */
    void clear() noexcept;

    /**
     * @brief Determines if a selection matches the filter.
     * @param selection the selection to check
     * @return "true" if the selection matches the filter, "false" otherwise
     */
    bool matches(const T& selection) const noexcept;
private:
    SelectionFilterType type;
    std::vector<T> values;
};


} // namespace dnd

template <typename T>
dnd::SelectionFilter<T>::SelectionFilter() noexcept : type(SelectionFilterType::NONE), values() {}

template <typename T>
dnd::SelectionFilterType dnd::SelectionFilter<T>::get_type() const noexcept {
    return type;
}

template <typename T>
void dnd::SelectionFilter<T>::set_type(SelectionFilterType type) noexcept {
    this->type = type;
}

template <typename T>
void dnd::SelectionFilter<T>::set_values(const std::vector<T>& values) noexcept {
    this->values = values;
}

template <typename T>
void dnd::SelectionFilter<T>::set_values(std::vector<T>&& values) noexcept {
    this->values = std::move(values);
}

template <typename T>
void dnd::SelectionFilter<T>::set(SelectionFilterType type, const std::vector<T>& values) noexcept {
    set_type(type);
    set_values(values);
}

template <typename T>
void dnd::SelectionFilter<T>::set(SelectionFilterType type, std::vector<T>&& values) noexcept {
    set_type(type);
    set_values(std::move(values));
}

template <typename T>
void dnd::SelectionFilter<T>::clear() noexcept {
    type = SelectionFilterType::NONE;
    values.clear();
}

template <typename T>
bool dnd::SelectionFilter<T>::matches(const T& selection) const noexcept {
    switch (type) {
        case SelectionFilterType::IS_IN:
            return std::find(values.begin(), values.end(), selection) != values.end();
        case SelectionFilterType::IS_NOT_IN:
            return std::find(values.begin(), values.end(), selection) == values.end();
        case SelectionFilterType::NONE:
            return true;
        default:
            return false;
    }
}

#endif // SELECTION_FILTERING_HPP_
