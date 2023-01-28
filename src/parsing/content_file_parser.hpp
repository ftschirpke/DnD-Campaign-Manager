#ifndef CONTENT_FILE_PARSER_HPP_
#define CONTENT_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

#include "parsing_types.hpp"

namespace dnd {

class ContentFileParser {
public:
    /**
     * @brief Opens a json file to be parsed and deserialises the JSON.
     * @param filepath the file to open
     * @return "true" if opening and deserialising was successful, "false" otherwise
     */
    virtual bool openJSON(const std::filesystem::directory_entry& filepath);
    /**
     * @brief Parses JSON content file.
     */
    virtual void parse() = 0;
    /**
     * @brief Checks whether parsed content is valid.
     * @return "true" if content is valid, "false" otherwise
     */
    virtual bool validate() const = 0;
    /**
     * @brief Saves the parsed content.
     */
    virtual void saveResult() = 0;
protected:
    nlohmann::json json_to_parse;
    std::filesystem::path filepath;
    /**
     * @brief This function is automatically called after successfully opening the JSON file to set the properties of
     * subparsers if necessary.
     */
    virtual void configureSubparsers();
};

} // namespace dnd

#endif // CONTENT_FILE_PARSER_HPP_
