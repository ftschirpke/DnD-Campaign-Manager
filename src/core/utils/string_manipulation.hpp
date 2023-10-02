#ifndef STRING_MANIPULATION_HPP_
#define STRING_MANIPULATION_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

/**
 * @brief Transform a string to lowercase
 * @param str the string to transform
 */
void string_lowercase_inplace(std::string& str);

/**
 * @brief Transform a string to uppercase
 * @param str the string to transform
 */
void string_uppercase_inplace(std::string& str);

/**
 * @brief Create a lowercase copy of a string
 * @param str the string to transform to lowercase
 * @return the lowercase string copy
 */
std::string string_lowercase_copy(const std::string& str);

/**
 * @brief Create an uppercase copy of a string
 * @param str the string to transform to uppercase
 * @return the uppercase string copy
 */
std::string string_uppercase_copy(const std::string& str);

/**
 * @brief Transform a snake_case string to capitalised words separated by spaces
 * @param str the string to transform
 */
void snake_case_to_spaced_words(std::string& str);

/**
 * @brief Create a string_view from string iterators
 * @param first the first iterator
 * @param last the last iterator
 * @return the created string_view
 */
std::string_view str_view(const std::string::const_iterator& first, const std::string::const_iterator& last);

} // namespace dnd

#endif // STRING_MANIPULATION_HPP_
