#ifndef CHARACTER_RACE_FILE_PARSER_HPP_
#define CHARACTER_RACE_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_race.hpp"
#include "models/features/feature.hpp"
#include "parsing/models/content_file_parser.hpp"

namespace dnd {

class CharacterRaceFileParser : public ContentFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const CharacterRace>>& results_map;
    std::string character_race_name;
    bool has_subraces;
    std::vector<std::shared_ptr<const Feature>> features;
public:
    CharacterRaceFileParser(std::unordered_map<std::string, std::shared_ptr<const CharacterRace>>& results_map);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
    void reset() override;
};

inline CharacterRaceFileParser::CharacterRaceFileParser(
    std::unordered_map<std::string, std::shared_ptr<const CharacterRace>>& results_map
)
    : results_map(results_map) {}

} // namespace dnd

#endif // CHARACTER_RACE_FILE_PARSER_HPP_
