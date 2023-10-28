#ifndef BOOL_FILTERING_HPP_
#define BOOL_FILTERING_HPP_

#include <dnd_config.hpp>

namespace dnd {

enum class BoolFilterType {
    IS_TRUE,
    IS_FALSE,
    NONE,
};

/**
 * @brief Determines if a boolean matches a filter.
 * @param boolean the boolean to check
 * @param type the type of the filter (determines how to check the boolean)
 * @return "true" if the boolean matches the filter, "false" otherwise
 */
bool matches_bool(bool boolean, BoolFilterType type) noexcept;

} // namespace dnd

#endif // BOOL_FILTERING_HPP_
