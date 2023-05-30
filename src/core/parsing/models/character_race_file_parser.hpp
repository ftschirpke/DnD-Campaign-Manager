#ifndef CHARACTER_RACE_FILE_PARSER_HPP_
#define CHARACTER_RACE_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <string>

#include "controllers/content_library.hpp"
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
     * @param filepath the file to parse
     * @param races the already-parsed races
     * @param groups the already-parsed groups
     */
    CharacterRaceFileParser(
        const std::filesystem::path& filepath, StoringContentLibrary<const CharacterRace>& races, const Groups& groups
    ) noexcept;
    /**
     * @brief Parses JSON file containing a race
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
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
    /**
     * @brief Returns the type of content that this parser parses - races
     * @return the type of content that this parser parses - races
     */
    virtual constexpr ParsingType getType() const override { return type; };
private:
    // the type of content that this parser parses - races
    static constexpr ParsingType type = ParsingType::RACE;
    // the name of the parsed race
    std::string character_race_name;
    // boolean for whether the parsed race has subraces
    bool has_subraces;
    // the already-parsed races to add the parsed race to
    StoringContentLibrary<const CharacterRace>& races;
    // a subparser used for parsing the race's features
    FeaturesParser features_parser;
};

inline CharacterRaceFileParser::CharacterRaceFileParser(
    const std::filesystem::path& filepath, StoringContentLibrary<const CharacterRace>& races, const Groups& groups
) noexcept
    : ContentFileParser(filepath), races(races), features_parser(type, filepath, groups) {}

} // namespace dnd

#endif // CHARACTER_RACE_FILE_PARSER_HPP_
