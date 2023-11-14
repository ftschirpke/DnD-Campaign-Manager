#ifndef STRING_MANIPULATION_HPP_
#define STRING_MANIPULATION_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

void string_lowercase_inplace(std::string& str);
void string_uppercase_inplace(std::string& str);
std::string string_lowercase_copy(const std::string& str);
std::string string_uppercase_copy(const std::string& str);
void snake_case_to_capitalized_spaced_words(std::string& str);
std::string_view str_view(const std::string::const_iterator& first, const std::string::const_iterator& last);

} // namespace dnd

#endif // STRING_MANIPULATION_HPP_
