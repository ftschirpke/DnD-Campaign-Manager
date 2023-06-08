#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include <core/controllers/content_holder.hpp>
#include <core/parsing/content_file_parser.hpp>
#include <core/parsing/parsing_types.hpp>

namespace dnd {

/**
 * @brief A class for parsing content
 */
class ContentParser {
public:
    /**
     * @brief Parses content from content_path for a certain campaign
     * @param content_path a path to the directory the content to be parsed is located in
     * @param campaign_dir_name the name of the campaign-specific directory
     * @return the parsed content
     * @throws parsing_error if any error occured while trying to parse the content
     */
    ContentHolder parse(const std::filesystem::path& content_path, const std::string& campaign_dir_name);
private:
    /**
     * @brief Deletes all parsed content and resets to a state as if just initialized
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
     * @throws std::logic_error if parsing type cannot be parsed as a single-file type
     * @throws parsing_error if any error occured while trying to parse the content
     */
    void parseAllOfSingleFileType(const ParsingType parsing_type);
    /**
     * @brief Handles parsing for a certain content type where the content is found within a directory in multiple files
     * @param parsing_type the content type to be parsed
     * @throws std::logic_error if parsing type cannot be parsed as a multi-file type
     * @throws parsing_error if any error occured while trying to parse the content
     */
    void parseAllOfMultiFileType(const ParsingType parsing_type);
    /**
     * @brief Handles parsing of one file by one parser
     * @param parser the parser
     * @throws parsing_error if any error occured while trying to parse the file with the parser
     */
    void parseFile(std::unique_ptr<ContentFileParser> parser);
    /**
     * @brief Creates a Parser for single-file parsing of a certain type
     * @param filepath the file to parse
     * @param parsing_type the content type to be parser
     * @return the single-file parser for that content type
     * @throws std::logic_error if no single-file parser exists for that content type
     */
    std::unique_ptr<ContentFileParser> createSingleFileParser(
        const std::filesystem::path filepath, const ParsingType parsing_type
    );
    /**
     * @brief Creates a Parser for multi-file parsing of a certain type
     * @param filepath the file to parse
     * @param parsing_type the content type to be parser
     * @return the multi-file parser for that content type
     * @throws std::logic_error if no single-file parser exists for that content type
     */
    std::unique_ptr<ContentFileParser> createMultiFileParser(
        const std::filesystem::path filepath, const ParsingType parsing_type
    );

    // the names of the files containing single-file content types
    static const std::array<std::pair<dnd::ParsingType, const char*>, 1> file_names;
    // the names of the directories containing multi-file content types
    static const std::array<std::pair<dnd::ParsingType, const char*>, 8> subdir_names;
    // the variable where the parsed content is accumulated
    ContentHolder parsed_content;
    // the content directories that should be parsed
    std::vector<std::filesystem::directory_entry> dirs_to_parse;
    // mutexes used to control the access to each of the content type maps within the parsed_content
    std::unordered_map<ParsingType, std::mutex> parsing_mutexes;
};

} // namespace dnd

#endif // CONTENT_PARSER_HPP_
