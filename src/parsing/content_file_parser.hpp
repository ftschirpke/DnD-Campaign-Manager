#ifndef CONTENT_FILE_PARSER_HPP_
#define CONTENT_FILE_PARSER_HPP_

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

namespace dnd {

// parse a simple, optional attribute from a json into the output variable
template <typename T>
void parseOptional(const nlohmann::json& json_to_parse, const char* attribute_name, T& output);

class ContentFileParser {
public:
    virtual bool openJSON(const std::filesystem::directory_entry& filepath);
    virtual void parse() = 0;
    virtual bool validate() const = 0;
    virtual void saveResult() = 0;
protected:
    nlohmann::json json_to_parse;
    std::filesystem::path filepath;
};

template <typename T>
inline void parseOptional(const nlohmann::json& json_to_parse, const char* attribute_name, T& output) {
    if (json_to_parse.contains(attribute_name)) {
        output = json_to_parse.at(attribute_name).get<T>();
    }
}

} // namespace dnd

#endif // CONTENT_FILE_PARSER_HPP_
