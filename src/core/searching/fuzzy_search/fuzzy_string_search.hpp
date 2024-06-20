#ifndef FUZZY_STRING_SEARCH_HPP_
#define FUZZY_STRING_SEARCH_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

int64_t fuzzy_match_string(const std::string& search_query, const std::string& string_to_match);

} // namespace dnd

#endif // FUZZY_STRING_SEARCH_HPP
