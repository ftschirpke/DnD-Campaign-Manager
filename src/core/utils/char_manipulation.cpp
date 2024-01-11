#include <dnd_config.hpp>

#include "char_manipulation.hpp"

#include <cctype>

namespace dnd {

unsigned char uchar_to_lowercase(unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); }

char char_to_lowercase(char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); }

unsigned char uchar_to_uppercase(unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); }

char char_to_uppercase(char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); }

} // namespace dnd
