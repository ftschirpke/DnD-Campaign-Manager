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

/**
 * @brief A class representing a filter for numbers.
 * @tparam T the type of the number
 */
template <typename T>
requires std::is_arithmetic_v<T>
class NumberFilter {
public:
    NumberFilter() noexcept;

    /**
     * @brief Determines if the filter is set.
     * @return "true" if the filter is set, "false" otherwise
     * @details A filter is set if the type is not "NONE".
     */
    bool is_set() const noexcept;
    /**
     * @brief Get the type of the filter.
     * @return the type of the filter
     */
    NumberFilterType get_type() const noexcept;
    /**
     * @brief Get the value of the filter.
     * @return the value of the filter
     */
    T get_value() const noexcept;
    /**
     * @brief Set the type of the filter.
     * @param type the type of the filter
     */
    void set_type(NumberFilterType type) noexcept;
    /**
     * @brief Set the value of the filter.
     * @param value the value of the filter
     */
    void set_value(T value) noexcept;
    /**
     * @brief Set the filter.
     * @param type the type of the filter
     * @param value the value of the filter
     */
    void set(NumberFilterType type, T value) noexcept;
    /**
     * @brief Clears the filter.
     */
    void clear() noexcept;

    /**
     * @brief Determines if a boolean matches the filter.
     * @param boolean the boolean to check
     * @return "true" if the boolean matches the filter, "false" otherwise
     */
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
void dnd::NumberFilter<T>::set_type(NumberFilterType type) noexcept {
    this->type = type;
}

template <typename T>
requires std::is_arithmetic_v<T>
void dnd::NumberFilter<T>::set_value(T value) noexcept {
    this->value = value;
}

template <typename T>
requires std::is_arithmetic_v<T>
void dnd::NumberFilter<T>::set(NumberFilterType type, T value) noexcept {
    set_type(type);
    set_value(value);
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
