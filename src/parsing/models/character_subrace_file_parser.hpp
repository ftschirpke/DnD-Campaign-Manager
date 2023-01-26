#ifndef CHARACTER_SUBRACE_FILE_PARSER_HPP_
#define CHARACTER_SUBRACE_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_race.hpp"
#include "models/effect_holder/feature.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

class CharacterSubraceFileParser : public FeatureHolderFileParser {
public:
    CharacterSubraceFileParser(
        std::unordered_map<std::string, const CharacterSubrace>& results, const Groups& groups,
        const std::unordered_map<std::string, const CharacterRace>& races
    ) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    std::unordered_map<std::string, const CharacterSubrace>& results;
    const std::unordered_map<std::string, const CharacterRace>& races;
    std::string character_subrace_name, race_name;
};

inline CharacterSubraceFileParser::CharacterSubraceFileParser(
    std::unordered_map<std::string, const CharacterSubrace>& results, const Groups& groups,
    const std::unordered_map<std::string, const CharacterRace>& races
) noexcept
    : FeatureHolderFileParser(groups), results(results), races(races) {}

} // namespace dnd

#endif // CHARACTER_SUBRACE_FILE_PARSER_HPP_
