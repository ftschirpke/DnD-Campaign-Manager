#ifndef CHECK_TEXT_HPP_
#define CHECK_TEXT_HPP_

#include <string>

namespace dnd {

std::string checked_string(std::string&& str);

inline std::string checked_string(std::string::const_iterator start, std::string::const_iterator end) {
    return checked_string(std::string(start, end));
}

} // namespace dnd

#endif // CHECK_TEXT_HPP_
