#include <dnd_config.hpp>

#include "string_manipulation.hpp"

#include <algorithm>
#include <string>
#include <string_view>

#include <core/utils/char_manipulation.hpp>

void dnd::string_to_lowercase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), uchar_to_lowercase);
}

void dnd::string_to_uppercase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), uchar_to_uppercase);
}

std::string dnd::string_lowercase_copy(const std::string& str) {
    std::string lower_str = str;
    string_to_lowercase(lower_str);
    return lower_str;
}

std::string dnd::string_uppercase_copy(const std::string& str) {
    std::string upper_str = str;
    string_to_uppercase(upper_str);
    return upper_str;
}

void dnd::snake_case_to_spaced_words(std::string& str) {
    bool first_in_word = true;
    for (auto& c : str) {
        if (c == '_') {
            c = ' ';
            first_in_word = true;
        }
        if (first_in_word) {
            c = char_to_uppercase(c);
            first_in_word = false;
        }
    }
}

std::string_view dnd::str_view(const std::string::const_iterator& first, const std::string::const_iterator& last) {
#ifdef __APPLE__
    auto it = first;
    size_t size = 0;
    while (it != last) {
        ++it;
        ++size;
    }
    return std::string_view(&*first, size);
#else
    return std::string_view(first, last);
#endif
}
