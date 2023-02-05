#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

#include "dnd_config.hpp"

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "controllers/content.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/parsing_types.hpp"

namespace dnd {

/**
 * @brief A class for parsing Content
 */
class ContentParser {
public:
    /**
     * @brief Parses content from content_path for a certain campaign
     * @param content_path a path to the directory the content to be parsed is located in
     * @param campaign_dir_name the name of the campaign-specific directory
     * @return
     */
    Content parse(const std::filesystem::path& content_path, const std::string& campaign_dir_name);
private:
    /**
     * @brief Deletes all parsed content and resets to a state as if just initialised
     */
    void reset() noexcept;
    /**
     * @brief Handles parsing of all files for a certain content type
     * @param parsing_type the content type to be parsed
     */
    void parseAllOfType(const ParsingType parsing_type);
    /**
     * @brief Handles parsing for a certain content type where the content is all in one file
     * @param parsing_type the content type to be parsed
     */
    void parseAllOfSingleFileType(const ParsingType parsing_type);
    /**
     * @brief Handles parsing for a certain content type where the content is found within a directory in multiple files
     * @param parsing_type the content type to be parsed
     */
    void parseAllOfMultiFileType(const ParsingType parsing_type);
    /**
     * @brief Handles parsing of one file given the way of how it should be parsed
     * @param file the file path
     * @param parsing_type the type of content in the file that should be parsed
     * @param multi_file "true" if the content type should be parsed in a multi-file way, "false" otherwise
     */
    void parseFileOfType(const std::filesystem::directory_entry& file, const ParsingType parsing_type, bool multi_file);
    /**
     * @brief Creates a Parser for single-file parsing of a certain type
     * @param parsing_type the content type to be parser
     * @return the single-file parser for that content type
     * @throws std::logic_error if no single-file parser exists for that content type
     */
    std::unique_ptr<ContentFileParser> createSingleFileParserForType(const ParsingType parsing_type);
    /**
     * @brief Creates a Parser for multi-file parsing of a certain type
     * @param parsing_type the content type to be parser
     * @return the multi-file parser for that content type
     * @throws std::logic_error if no single-file parser exists for that content type
     */
    std::unique_ptr<ContentFileParser> createMultiFileParserForType(const ParsingType parsing_type);

    // the names of the files containing single-file content types
    static const std::unordered_map<ParsingType, std::string> file_names;
    // the names of the directories containing multi-file content types
    static const std::unordered_map<ParsingType, std::string> subdir_names;
    // the variable where the parsed content is accumulated
    Content parsed_content;
    // the content directories that should be parsed
    std::vector<std::filesystem::directory_entry> dirs_to_parse;
    // mutexes used to control the access to each of the content type maps within the parsed_content
    std::unordered_map<ParsingType, std::mutex> parsing_mutexes;
};

inline const std::unordered_map<ParsingType, std::string> ContentParser::file_names = {
    {ParsingType::GROUP, "groups"},
};

inline const std::unordered_map<ParsingType, std::string> ContentParser::subdir_names = {
    {ParsingType::GROUP, "groups"},        {ParsingType::CHARACTER, "characters"}, {ParsingType::CLASS, "classes"},
    {ParsingType::SUBCLASS, "subclasses"}, {ParsingType::RACE, "races"},           {ParsingType::SUBRACE, "subraces"},
    {ParsingType::SPELL, "spells"},
};

} // namespace dnd

#endif // CONTENT_PARSER_HPP_
