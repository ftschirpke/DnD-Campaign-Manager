#ifndef PARSE_OPTIONALS_HPP_
#define PARSE_OPTIONALS_HPP_

#include <dnd_config.hpp>

#include <nlohmann/json.hpp>

namespace dnd {

/**
 * @brief Parse an optional attribute from a json and adding it into the output variable if it exists
 * @tparam T type the attribute value is supposed to be
 * @param json_to_parse the json the attribute is supposed to be in
 * @param attribute_name the name of the attribute
 * @param output the output to write the attribute value to
 */
template <typename T>
inline void parseOptional(const nlohmann::json& json_to_parse, const char* attribute_name, T& output) {
    if (json_to_parse.contains(attribute_name)) {
        output = json_to_parse.at(attribute_name).get<T>();
    }
}

} // namespace dnd

#endif // PARSE_OPTIONALS_HPP_
