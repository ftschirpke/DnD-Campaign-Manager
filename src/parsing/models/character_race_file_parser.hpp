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

/**
 * @brief A class for parsing character races (multi-file)
 */
class CharacterRaceFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a CharacterRaceFileParser
     * @param races the already-parsed races
     * @param groups the already-parsed groups
     */
    CharacterRaceFileParser(std::unordered_map<std::string, const CharacterRace>& races, const Groups& groups) noexcept;
    /**
     * @brief Parses JSON file containing a race.
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed race is valid
     * @return "true" if the race is valid, "false" otherwise
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed race
     */
    virtual void saveResult() override;
private:
    /**
     * @brief Configures the subparsers used
     */
    virtual void configureSubparsers() override;

    // the type of content that this parser parses - races
    static const ParsingType type;
    // the name of the parsed race
    std::string character_race_name;
    // boolean for whether the parsed race has subraces
    bool has_subraces;
    // the already-parsed races to add the parsed race to
    std::unordered_map<std::string, const CharacterRace>& races;
    // a subparser used for parsing the race's features
    FeaturesParser features_parser;
};

inline CharacterRaceFileParser::CharacterRaceFileParser(
    std::unordered_map<std::string, const CharacterRace>& races, const Groups& groups
) noexcept
    : ContentFileParser(), races(races), features_parser(groups) {}

inline void CharacterRaceFileParser::configureSubparsers() { features_parser.configure(type, filepath); }

} // namespace dnd

#endif // CHARACTER_RACE_FILE_PARSER_HPP_
