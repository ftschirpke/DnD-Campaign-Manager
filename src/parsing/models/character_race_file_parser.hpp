#ifndef CHARACTER_RACE_FILE_PARSER_HPP_
#define CHARACTER_RACE_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_race.hpp"
#include "models/features/feature.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

class CharacterRaceFileParser : public FeatureHolderFileParser {
private:
    std::unordered_map<std::string, const CharacterRace>& results;
    std::string character_race_name;
    bool has_subraces;
public:
    CharacterRaceFileParser(std::unordered_map<std::string, const CharacterRace>& results);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
};

inline CharacterRaceFileParser::CharacterRaceFileParser(std::unordered_map<std::string, const CharacterRace>& results)
    : results(results) {}

} // namespace dnd

#endif // CHARACTER_RACE_FILE_PARSER_HPP_
