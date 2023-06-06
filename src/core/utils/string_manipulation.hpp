#ifndef STRING_MANIPULATION_HPP_
#define STRING_MANIPULATION_HPP_

#include <dnd_config.hpp>

#include <algorithm>
#include <string>

#include <core/utils/char_manipulation.hpp>

namespace dnd {

/**
 * @brief Transform a string to lowercase
 * @param str the string to transform
 */
inline void string_to_lowercase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), uchar_to_lowercase);
}

/**
 * @brief Transform a string to uppercase
 * @param str the string to transform
 */
inline void string_to_uppercase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), uchar_to_uppercase);
}

/**
 * @brief Create a lowercase copy of a string
 * @param str the string to transform to lowercase
 * @return the lowercase string copy
 */
inline std::string string_lowercase_copy(const std::string& str) {
    std::string lower_str = str;
    string_to_lowercase(lower_str);
    return lower_str;
}

/**
 * @brief Create an uppercase copy of a string
 * @param str the string to transform to uppercase
 * @return the uppercase string copy
 */
inline std::string string_uppercase_copy(const std::string& str) {
    std::string upper_str = str;
    string_to_uppercase(upper_str);
    return upper_str;
}

} // namespace dnd

#endif // STRING_MANIPULATION_HPP_
