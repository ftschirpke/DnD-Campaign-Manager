#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

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

class ContentParser {
public:
    // parsing content from content_path for a certain campaign
    Content parse(const std::filesystem::path& content_path, const std::string& campaign_dir_name);
private:
    static const std::unordered_map<ParsingType, std::string> file_names;
    static const std::unordered_map<ParsingType, std::string> subdir_names;
    Content parsed_content;
    std::vector<std::filesystem::directory_entry> dirs_to_parse;
    // mutexes used to control the access to each of the content type maps within the parsed_content
    std::unordered_map<ParsingType, std::mutex> parsing_mutexes;
    void reset() noexcept;
    void parseAllOfType(const ParsingType parsing_type);
    void parseAllOfSingleFileType(const ParsingType parsing_type);
    void parseAllOfMultiFileType(const ParsingType parsing_type);
    void parseFileOfType(const std::filesystem::directory_entry& file, const ParsingType parsing_type, bool multi_file);
    std::unique_ptr<ContentFileParser> createSingleFileParserForType(const ParsingType parsing_type);
    std::unique_ptr<ContentFileParser> createMultiFileParserForType(const ParsingType parsing_type);
    std::unique_ptr<ContentFileParser> createGeneralParserForType(const ParsingType parsing_type);
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
