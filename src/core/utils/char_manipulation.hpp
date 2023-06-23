#ifndef CHAR_MANIPULATION_HPP_
#define CHAR_MANIPULATION_HPP_

#include <dnd_config.hpp>

namespace dnd {

/**
 * @brief A type-safe version of std::tolower using unsigned char
 * @param c the character to tranform to lowercase
 * @return the lowercase character
 */
unsigned char uchar_to_lowercase(unsigned char c);

/**
 * @brief A type-safe version of std::tolower using char
 * @param c the character to tranform to lowercase
 * @return the lowercase character
 */
char char_to_lowercase(char c);

/**
 * @brief A type-safe version of std::toupper using unsigned char
 * @param c the character to tranform to uppercase
 * @return the uppercase character
 */
unsigned char uchar_to_uppercase(unsigned char c);

/**
 * @brief A type-safe version of std::toupper using char
 * @param c the character to tranform to uppercase
 * @return the uppercase character
 */
char char_to_uppercase(char c);

} // namespace dnd

#endif // CHAR_MANIPULATION_HPP_
