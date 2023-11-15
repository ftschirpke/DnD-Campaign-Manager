#ifndef NUMBER_FILTERING_HPP_
#define NUMBER_FILTERING_HPP_

#include <dnd_config.hpp>

#include <type_traits>

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
    NumberFilter() noexcept;
    bool is_set() const noexcept;
    NumberFilterType get_type() const noexcept;
    T get_value() const noexcept;
    void set_type(NumberFilterType new_type) noexcept;
    void set_value(T new_value) noexcept;
    void set(NumberFilterType type, T new_value) noexcept;
    void clear() noexcept;
    bool matches(T number) const noexcept;
private:
    NumberFilterType type;
    T value;
};

} // namespace dnd

template <typename T>
requires std::is_arithmetic_v<T>
dnd::NumberFilter<T>::NumberFilter() noexcept : type(NumberFilterType::NONE), value(0) {}

template <typename T>
requires std::is_arithmetic_v<T>
bool dnd::NumberFilter<T>::is_set() const noexcept {
    return type != NumberFilterType::NONE;
}

template <typename T>
requires std::is_arithmetic_v<T>
dnd::NumberFilterType dnd::NumberFilter<T>::get_type() const noexcept {
    return type;
}

template <typename T>
requires std::is_arithmetic_v<T>
T dnd::NumberFilter<T>::get_value() const noexcept {
    return value;
}

template <typename T>
requires std::is_arithmetic_v<T>
void dnd::NumberFilter<T>::set_type(NumberFilterType new_type) noexcept {
    type = new_type;
}

template <typename T>
requires std::is_arithmetic_v<T>
void dnd::NumberFilter<T>::set_value(T new_value) noexcept {
    value = new_value;
}

template <typename T>
requires std::is_arithmetic_v<T>
void dnd::NumberFilter<T>::set(NumberFilterType new_type, T new_value) noexcept {
    set_type(new_type);
    set_value(new_value);
}

template <typename T>
requires std::is_arithmetic_v<T>
void dnd::NumberFilter<T>::clear() noexcept {
    type = NumberFilterType::NONE;
    value = 0;
}

template <typename T>
requires std::is_arithmetic_v<T>
bool dnd::NumberFilter<T>::matches(T number) const noexcept {
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
        default:
            return false;
    }
}


#endif // NUMBER_FILTERING_HPP_
