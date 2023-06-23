#include <dnd_config.hpp>

#include "char_manipulation.hpp"

#include <cctype>

/**
 * @brief A type-safe version of std::tolower using unsigned char
 * @param c the character to tranform to lowercase
 * @return the lowercase character
 */
unsigned char dnd::uchar_to_lowercase(unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); }

/**
 * @brief A type-safe version of std::tolower using char
 * @param c the character to tranform to lowercase
 * @return the lowercase character
 */
char dnd::char_to_lowercase(char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); }

/**
 * @brief A type-safe version of std::toupper using unsigned char
 * @param c the character to tranform to uppercase
 * @return the uppercase character
 */
unsigned char dnd::uchar_to_uppercase(unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); }

/**
 * @brief A type-safe version of std::toupper using char
 * @param c the character to tranform to uppercase
 * @return the uppercase character
 */
char dnd::char_to_uppercase(char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); }
