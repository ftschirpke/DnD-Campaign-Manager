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

/**
 * @brief A class for parsing character subraces (multi-file)
 */
class CharacterSubraceFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a CharacterSubraceFileParser
     * @param subraces the already-parsed subraces
     * @param groups the already-parsed groups
     * @param races the already-parsed races
     */
    CharacterSubraceFileParser(
        std::unordered_map<std::string, const CharacterSubrace>& subraces, const Groups& groups,
        const std::unordered_map<std::string, const CharacterRace>& races
    ) noexcept;
    /**
     * @brief Parses JSON file containing a subrace.
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed subrace is valid
     * @return "true" if the subrace is valid, "false" otherwise
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed subrace
     */
    virtual void saveResult() override;
private:
    /**
     * @brief Configures the subparsers used
     */
    virtual void configureSubparsers() override;

    // the type of content that this parser parses - subraces
    static const ParsingType type;
    // the name of the parsed subrace
    std::string character_subrace_name;
    // the name of the race for the parsed subrace
    std::string race_name;
    // the already-parsed subraces to add the parsed subrace to
    std::unordered_map<std::string, const CharacterSubrace>& subraces;
    // the already-parsed races to check whether such a race exists and has subraces
    const std::unordered_map<std::string, const CharacterRace>& races;
    // a subparser used for parsing the subrace's features
    FeaturesParser features_parser;
};

inline CharacterSubraceFileParser::CharacterSubraceFileParser(
    std::unordered_map<std::string, const CharacterSubrace>& subraces, const Groups& groups,
    const std::unordered_map<std::string, const CharacterRace>& races
) noexcept
    : ContentFileParser(), subraces(subraces), races(races), features_parser(groups) {}

inline void CharacterSubraceFileParser::configureSubparsers() { features_parser.configure(type, filepath); }

} // namespace dnd

#endif // CHARACTER_SUBRACE_FILE_PARSER_HPP_
