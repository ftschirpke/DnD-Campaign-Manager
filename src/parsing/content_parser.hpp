#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

#include <filesystem>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "controllers/content.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/spell.hpp"
#include "parsing/models/character_file_parser.hpp"
#include "parsing/parsing_types.hpp"

namespace dnd {

class ContentParser {
public:
    // parsing content from content_path for a certain campaign
    Content parse(const std::filesystem::path& content_path, const std::string& campaign_dir_name);
private:
    static const std::unordered_map<ParsingType, std::string> subdir_names;
    Content parsed_content;
    // mutexes used to control the access to each of the content type maps within the parsed_content
    std::unordered_map<ParsingType, std::mutex> parsing_mutexes;
    void resetParsed();
    void parseFileOfType(const ParsingType parsing_type, const std::filesystem::directory_entry& file);
    void parseAllOfType(
        const ParsingType parsing_type, const std::vector<std::filesystem::directory_entry>& dirs_to_parse
    );
    std::unique_ptr<ContentFileParser> createParser(const ParsingType parsing_type);
};

inline const std::unordered_map<ParsingType, std::string> ContentParser::subdir_names = {
    {ParsingType::CHARACTER, "characters"}, {ParsingType::CLASS, "classes"},    {ParsingType::SUBCLASS, "subclasses"},
    {ParsingType::RACE, "races"},           {ParsingType::SUBRACE, "subraces"}, {ParsingType::SPELL, "spells"},
};

} // namespace dnd

#endif // CONTENT_PARSER_HPP_
