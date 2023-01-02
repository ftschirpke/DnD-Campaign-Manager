#ifndef CHARACTER_SUBRACE_FILE_PARSER_HPP_
#define CHARACTER_SUBRACE_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_race.hpp"
#include "models/features/feature.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

class CharacterSubraceFileParser : public FeatureHolderFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubrace>>& results;
    const std::unordered_map<std::string, std::shared_ptr<const CharacterRace>>& races;
    std::string character_subrace_name, race_name;
    std::vector<std::shared_ptr<const Feature>> features;
public:
    CharacterSubraceFileParser(
        std::unordered_map<std::string, std::shared_ptr<const CharacterSubrace>>& results,
        const std::unordered_map<std::string, std::shared_ptr<const CharacterRace>>& races
    );
    void parse() override;
    bool validate() const override;
    void saveResult() override;
};

inline CharacterSubraceFileParser::CharacterSubraceFileParser(
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubrace>>& results,
    const std::unordered_map<std::string, std::shared_ptr<const CharacterRace>>& races
)
    : results(results), races(races) {}

} // namespace dnd

#endif // CHARACTER_SUBRACE_FILE_PARSER_HPP_
