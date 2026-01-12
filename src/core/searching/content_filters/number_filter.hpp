#ifndef NUMBER_FILTERING_HPP_
#define NUMBER_FILTERING_HPP_

#include <dnd_config.hpp>

#include <cassert>
#include <type_traits>
#include <utility>

namespace dnd {

enum class NumberFilterType {
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    NONE,
};

template <typename T>
requires std::is_arithmetic_v<T>
class NumberFilter {
public:
    NumberFilter();
    bool is_set() const;
    NumberFilterType get_type() const;
    T get_value() const;
    void set_type(NumberFilterType new_type);
    void set_value(T new_value);
    void set(NumberFilterType type, T new_value);
    void clear();
    bool matches(T number) const;
private:
    NumberFilterType type;
    T value;
};


// === IMPLEMENTATION ===

template <typename T>
requires std::is_arithmetic_v<T>
NumberFilter<T>::NumberFilter() : type(NumberFilterType::NONE), value(0) {}

template <typename T>
requires std::is_arithmetic_v<T>
bool NumberFilter<T>::is_set() const {
    return type != NumberFilterType::NONE;
}

template <typename T>
requires std::is_arithmetic_v<T>
NumberFilterType NumberFilter<T>::get_type() const {
    return type;
}

template <typename T>
requires std::is_arithmetic_v<T>
T NumberFilter<T>::get_value() const {
    return value;
}

template <typename T>
requires std::is_arithmetic_v<T>
void NumberFilter<T>::set_type(NumberFilterType new_type) {
    type = new_type;
}

template <typename T>
requires std::is_arithmetic_v<T>
void NumberFilter<T>::set_value(T new_value) {
    value = new_value;
}

template <typename T>
requires std::is_arithmetic_v<T>
void NumberFilter<T>::set(NumberFilterType new_type, T new_value) {
    set_type(new_type);
    set_value(new_value);
}

template <typename T>
requires std::is_arithmetic_v<T>
void NumberFilter<T>::clear() {
    type = NumberFilterType::NONE;
    value = 0;
}

template <typename T>
requires std::is_arithmetic_v<T>
bool NumberFilter<T>::matches(T number) const {
    switch (type) {
        case NumberFilterType::EQUAL:
            return number == value;
        case NumberFilterType::NOT_EQUAL:
            return number != value;
        case NumberFilterType::LESS_THAN:
            return number < value;
        case NumberFilterType::LESS_THAN_OR_EQUAL:
            return number <= value;
        case NumberFilterType::GREATER_THAN:
            return number > value;
        case NumberFilterType::GREATER_THAN_OR_EQUAL:
            return number >= value;
        case NumberFilterType::NONE:
            return true;
    }
    std::unreachable();
}

} // namespace dnd

#endif // NUMBER_FILTERING_HPP_
