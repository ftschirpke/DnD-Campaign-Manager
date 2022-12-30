#ifndef CHARACTER_SUBRACE_FILE_PARSER_HPP_
#define CHARACTER_SUBRACE_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "models/character_race.hpp"
#include "parsing/models/content_file_parser.hpp"

namespace dnd {

class CharacterSubraceFileParser : public ContentFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubrace>>& results_map;
    std::string name;
    bool has_subraces;
    std::vector<std::shared_ptr<const Feature>> features;
public:
    CharacterSubraceFileParser(std::unordered_map<std::string, std::shared_ptr<const CharacterSubrace>>& results_map);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
    void reset() override;
};

inline CharacterSubraceFileParser::CharacterSubraceFileParser(
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubrace>>& results_map
)
    : results_map(results_map) {}

} // namespace dnd

#endif // CHARACTER_SUBRACE_FILE_PARSER_HPP_
