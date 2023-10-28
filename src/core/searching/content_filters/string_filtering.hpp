#ifndef STRING_FILTERING_HPP_
#define STRING_FILTERING_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

enum class StringFilterType {
    EQUAL,
    NOT_EQUAL,
    CONTAINS,
    NOT_CONTAINS,
    STARTS_WITH,
    NOT_STARTS_WITH,
    ENDS_WITH,
    NOT_ENDS_WITH,
    NONE,
};

/**
 * @brief Determines if a string matches a filter.
 * @param str the string to check
 * @param type the type of the filter (determines how to check the string)
 * @param filter the filter to check against
 * @return "true" if the string matches the filter, "false" otherwise
 */
bool matches_string(const std::string& str, StringFilterType type, const std::string& filter) noexcept;

} // namespace dnd

#endif // STRING_FILTERING_HPP_
