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

template <typename T>
class SelectionFilter {
public:
    SelectionFilter() noexcept;
    bool is_set() const noexcept;
    SelectionFilterType get_type() const noexcept;
    const std::vector<T>& get_values() const noexcept;
    void set_type(SelectionFilterType new_type) noexcept;
    void set_values(const std::vector<T>& values) noexcept;
    void set_values(std::vector<T>&& values) noexcept;
    void set(SelectionFilterType new_type, const std::vector<T>& values) noexcept;
    void set(SelectionFilterType new_type, std::vector<T>&& new_values) noexcept;
    void clear() noexcept;
    bool matches(const T& selection) const noexcept;
private:
    SelectionFilterType type;
    std::vector<T> values;
};


} // namespace dnd

template <typename T>
dnd::SelectionFilter<T>::SelectionFilter() noexcept : type(SelectionFilterType::NONE), values() {}

template <typename T>
bool dnd::SelectionFilter<T>::is_set() const noexcept {
    return type != SelectionFilterType::NONE;
}

template <typename T>
dnd::SelectionFilterType dnd::SelectionFilter<T>::get_type() const noexcept {
    return type;
}

template <typename T>
const std::vector<T>& dnd::SelectionFilter<T>::get_values() const noexcept {
    return values;
}

template <typename T>
void dnd::SelectionFilter<T>::set_type(SelectionFilterType new_type) noexcept {
    type = new_type;
}

template <typename T>
void dnd::SelectionFilter<T>::set_values(const std::vector<T>& new_values) noexcept {
    values = new_values;
}

template <typename T>
void dnd::SelectionFilter<T>::set_values(std::vector<T>&& new_values) noexcept {
    values = std::move(new_values);
}

template <typename T>
void dnd::SelectionFilter<T>::set(SelectionFilterType new_type, const std::vector<T>& new_values) noexcept {
    set_type(new_type);
    set_values(new_values);
}

template <typename T>
void dnd::SelectionFilter<T>::set(SelectionFilterType new_type, std::vector<T>&& new_values) noexcept {
    set_type(new_type);
    set_values(std::move(new_values));
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
