#ifndef CHAR_MANIPULATION_HPP_
#define CHAR_MANIPULATION_HPP_

#include <dnd_config.hpp>

#include <cctype>

namespace dnd {

/**
 * @brief A type-safe version of std::tolower using unsigned char
 * @param c the character to tranform to lowercase
 * @return the lowercase character
 */
inline unsigned char uchar_to_lowercase(unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); }

/**
 * @brief A type-safe version of std::tolower using char
 * @param c the character to tranform to lowercase
 * @return the lowercase character
 */
inline char char_to_lowercase(char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); }

/**
 * @brief A type-safe version of std::toupper using unsigned char
 * @param c the character to tranform to uppercase
 * @return the uppercase character
 */
inline unsigned char uchar_to_uppercase(unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); }

/**
 * @brief A type-safe version of std::toupper using char
 * @param c the character to tranform to uppercase
 * @return the uppercase character
 */
inline char char_to_uppercase(char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); }

} // namespace dnd

#endif // CHAR_MANIPULATION_HPP_
