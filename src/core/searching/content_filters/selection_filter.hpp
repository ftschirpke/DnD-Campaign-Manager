#ifndef SELECTION_FILTERING_HPP_
#define SELECTION_FILTERING_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <cassert>
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
    SelectionFilter();
    bool is_set() const;
    SelectionFilterType get_type() const;
    const std::vector<T>& get_values() const;
    void set_type(SelectionFilterType new_type);
    void set_values(const std::vector<T>& values);
    void set_values(std::vector<T>&& values);
    void set(SelectionFilterType new_type, const std::vector<T>& values);
    void set(SelectionFilterType new_type, std::vector<T>&& new_values);
    void clear();
    bool matches(const T& selection) const;
private:
    SelectionFilterType type;
    std::vector<T> values;
};


// === IMPLEMENTATION ===

template <typename T>
SelectionFilter<T>::SelectionFilter() : type(SelectionFilterType::NONE), values() {}

template <typename T>
bool SelectionFilter<T>::is_set() const {
    return type != SelectionFilterType::NONE;
}

template <typename T>
SelectionFilterType SelectionFilter<T>::get_type() const {
    return type;
}

template <typename T>
const std::vector<T>& SelectionFilter<T>::get_values() const {
    return values;
}

template <typename T>
void SelectionFilter<T>::set_type(SelectionFilterType new_type) {
    type = new_type;
}

template <typename T>
void SelectionFilter<T>::set_values(const std::vector<T>& new_values) {
    values = new_values;
}

template <typename T>
void SelectionFilter<T>::set_values(std::vector<T>&& new_values) {
    values = std::move(new_values);
}

template <typename T>
void SelectionFilter<T>::set(SelectionFilterType new_type, const std::vector<T>& new_values) {
    set_type(new_type);
    set_values(new_values);
}

template <typename T>
void SelectionFilter<T>::set(SelectionFilterType new_type, std::vector<T>&& new_values) {
    set_type(new_type);
    set_values(std::move(new_values));
}

template <typename T>
void SelectionFilter<T>::clear() {
    type = SelectionFilterType::NONE;
    values.clear();
}

template <typename T>
bool SelectionFilter<T>::matches(const T& selection) const {
    switch (type) {
        case SelectionFilterType::IS_IN:
            return std::find(values.begin(), values.end(), selection) != values.end();
        case SelectionFilterType::IS_NOT_IN:
            return std::find(values.begin(), values.end(), selection) == values.end();
        case SelectionFilterType::NONE:
            return true;
    }
    std::unreachable();
}

} // namespace dnd

#endif // SELECTION_FILTERING_HPP_
