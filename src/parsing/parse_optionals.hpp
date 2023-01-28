#ifndef PARSE_OPTIONALS_HPP_
#define PARSE_OPTIONALS_HPP_

#include "dnd_config.hpp"

#include <nlohmann/json.hpp>

namespace dnd {

// parse a simple, optional attribute from a json into the output variable
template <typename T>
inline void parseOptional(const nlohmann::json& json_to_parse, const char* attribute_name, T& output) {
    if (json_to_parse.contains(attribute_name)) {
        output = json_to_parse.at(attribute_name).get<T>();
    }
}

} // namespace dnd

#endif // PARSE_OPTIONALS_HPP_
