#include <dnd_config.hpp>

#include "number_filtering.hpp"

#include <type_traits>

template <typename T>
requires std::is_arithmetic_v<T>
bool dnd::matches_number(T number, NumberFilterType type, T filter) noexcept {
    switch (type) {
        case NumberFilterType::EQUAL:
            return number == filter;
        case NumberFilterType::NOT_EQUAL:
            return number != filter;
        case NumberFilterType::LESS_THAN:
            return number < filter;
        case NumberFilterType::LESS_THAN_OR_EQUAL:
            return number <= filter;
        case NumberFilterType::GREATER_THAN:
            return number > filter;
        case NumberFilterType::GREATER_THAN_OR_EQUAL:
            return number >= filter;
        case NumberFilterType::NONE:
            return true;
        default:
            return false;
    }
}

template bool dnd::matches_number<int>(int, NumberFilterType, int) noexcept;
template bool dnd::matches_number<float>(float, NumberFilterType, float) noexcept;
