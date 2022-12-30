#ifndef CONTENT_FILE_PARSER_HPP_
#define CONTENT_FILE_PARSER_HPP_

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

namespace dnd {

class ContentFileParser {
protected:
    nlohmann::json json_to_parse;
    std::string filename;
public:
    virtual bool openJSON(const std::filesystem::directory_entry& filepath);
    virtual void parse() = 0;
    virtual bool validate() const = 0;
    virtual void saveResult() = 0;
    virtual void reset() = 0;
};

} // namespace dnd

#endif // CONTENT_FILE_PARSER_HPP_
