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
 * @brief Determines if a number matches a filter.
 * @tparam T the type of the number
 * @param number the number to check
 * @param type the type of the filter (determines how to check the number)
 * @param filter the filter to check against
 * @return "true" if the number matches the filter, "false" otherwise
 * @note Use this function with the following types: int, float
 * (any other types will probably result in a compile error)
 */
template <typename T>
requires std::is_arithmetic_v<T>
bool matches_number(T number, NumberFilterType type, T filter) noexcept;

} // namespace dnd

#endif // NUMBER_FILTERING_HPP_
