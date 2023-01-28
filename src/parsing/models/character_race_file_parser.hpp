#ifndef CHARACTER_RACE_FILE_PARSER_HPP_
#define CHARACTER_RACE_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>

#include "controllers/groups.hpp"
#include "models/character_race.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/features_parser.hpp"
#include "parsing/parsing_types.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

class CharacterRaceFileParser : public ContentFileParser {
public:
    CharacterRaceFileParser(
        std::unordered_map<std::string, const CharacterRace>& results, const Groups& groups
    ) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    static const ParsingType type;
    std::unordered_map<std::string, const CharacterRace>& results;
    std::string character_race_name;
    bool has_subraces;
    FeaturesParser features_parser;
    virtual void configureSubparsers();
};

inline const ParsingType CharacterRaceFileParser::type = ParsingType::RACE;

inline CharacterRaceFileParser::CharacterRaceFileParser(
    std::unordered_map<std::string, const CharacterRace>& results, const Groups& groups
) noexcept
    : ContentFileParser(), results(results), features_parser(groups) {}

inline void CharacterRaceFileParser::configureSubparsers() { features_parser.configure(type, filepath); }

} // namespace dnd

#endif // CHARACTER_RACE_FILE_PARSER_HPP_
