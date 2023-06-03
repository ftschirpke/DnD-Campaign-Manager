#ifndef CONTENT_FILE_PARSER_HPP_
#define CONTENT_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

#include "core/parsing/parsing_types.hpp"

namespace dnd {

/**
 * @brief A class is parsing a content JSON file.
 * It might use subparsers for smaller units of the file.
 */
class ContentFileParser {
public:
    /**
     * @brief Constructs a ContentFileParser for a given file
     * @param filepath the file to parse
     */
    ContentFileParser(const std::filesystem::path& filepath) noexcept;
    ContentFileParser(ContentFileParser&& other) noexcept = default;
    virtual ~ContentFileParser() noexcept = default;
    /**
     * @brief Opens the json file to be parsed and deserialises the JSON.
     * @return "true" if opening and deserialising was successful, "false" otherwise
     */
    virtual bool openJSON();
    /**
     * @brief Parses JSON content file.
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
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
    /**
     * @brief Returns the type of content that this parser parses
     * @return the type of content that this parser parses
     */
    virtual ParsingType getType() const = 0;

    // the file which is being parsed
    const std::filesystem::path filepath;
protected:
    // the json content that is being parsed
    nlohmann::ordered_json json_to_parse;
};

inline ContentFileParser::ContentFileParser(const std::filesystem::path& filepath) noexcept : filepath(filepath) {}

} // namespace dnd

#endif // CONTENT_FILE_PARSER_HPP_
