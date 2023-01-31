#ifndef CHARACTER_SUBRACE_FILE_PARSER_HPP_
#define CHARACTER_SUBRACE_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "controllers/groups.hpp"
#include "models/character_race.hpp"
#include "models/character_subrace.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/features_parser.hpp"
#include "parsing/parsing_types.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

class CharacterSubraceFileParser : public ContentFileParser {
public:
    CharacterSubraceFileParser(
        std::unordered_map<std::string, const CharacterSubrace>& subraces, const Groups& groups,
        const std::unordered_map<std::string, const CharacterRace>& races
    ) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    static const ParsingType type;
    std::unordered_map<std::string, const CharacterSubrace>& subraces;
    const std::unordered_map<std::string, const CharacterRace>& races;
    std::string character_subrace_name, race_name;
    FeaturesParser features_parser;
    virtual void configureSubparsers() override;
};

inline const ParsingType CharacterSubraceFileParser::type = ParsingType::SUBRACE;

inline CharacterSubraceFileParser::CharacterSubraceFileParser(
    std::unordered_map<std::string, const CharacterSubrace>& subraces, const Groups& groups,
    const std::unordered_map<std::string, const CharacterRace>& races
) noexcept
    : ContentFileParser(), subraces(subraces), races(races), features_parser(groups) {}

inline void CharacterSubraceFileParser::configureSubparsers() { features_parser.configure(type, filepath); }

} // namespace dnd

#endif // CHARACTER_SUBRACE_FILE_PARSER_HPP_
