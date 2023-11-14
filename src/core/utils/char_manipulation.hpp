#ifndef CHAR_MANIPULATION_HPP_
#define CHAR_MANIPULATION_HPP_

#include <dnd_config.hpp>

namespace dnd {

// type-safe char manipulation functions

unsigned char uchar_to_lowercase(unsigned char c);
char char_to_lowercase(char c);
unsigned char uchar_to_uppercase(unsigned char c);
char char_to_uppercase(char c);

} // namespace dnd

#endif // CHAR_MANIPULATION_HPP_
