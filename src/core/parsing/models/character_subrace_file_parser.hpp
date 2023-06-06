#ifndef CHARACTER_SUBRACE_FILE_PARSER_HPP_
#define CHARACTER_SUBRACE_FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/controllers/content_library.hpp>
#include <core/controllers/groups.hpp>
#include <core/models/character_race.hpp>
#include <core/models/character_subrace.hpp>
#include <core/parsing/content_file_parser.hpp>
#include <core/parsing/models/effect_holder/features_parser.hpp>
#include <core/parsing/parsing_types.hpp>
#include <core/parsing/subparser.hpp>

namespace dnd {

/**
 * @brief A class for parsing character subraces (multi-file)
 */
class CharacterSubraceFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a CharacterSubraceFileParser
     * @param filepath the file to parse
     * @param subraces the already-parsed subraces
     * @param groups the already-parsed groups
     * @param races the already-parsed races
     */
    CharacterSubraceFileParser(
        const std::filesystem::path& filepath, StorageContentLibrary<const CharacterSubrace>& subraces,
        const Groups& groups, const StorageContentLibrary<const CharacterRace>& races
    ) noexcept;
    /**
     * @brief Parses JSON file containing a subrace
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed subrace is valid
     * @return "true" if the subrace is valid, "false" otherwise
     * @throws invalid_attribute if the chosen corresponding race does not exist or does not have subraces
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed subrace
     */
    virtual void save_result() override;
    /**
     * @brief Returns the type of content that this parser parses - subraces
     * @return the type of content that this parser parses - subraces
     */
    virtual constexpr ParsingType get_type() const override { return type; };
private:
    // the type of content that this parser parses - subraces
    static constexpr ParsingType type = ParsingType::SUBRACE;
    // the name of the parsed subrace
    std::string character_subrace_name;
    // the name of the race for the parsed subrace
    std::string race_name;
    // the already-parsed subraces to add the parsed subrace to
    StorageContentLibrary<const CharacterSubrace>& subraces;
    // the already-parsed races to check whether such a race exists and has subraces
    const StorageContentLibrary<const CharacterRace>& races;
    // a subparser used for parsing the subrace's features
    FeaturesParser features_parser;
};

inline CharacterSubraceFileParser::CharacterSubraceFileParser(
    const std::filesystem::path& filepath, StorageContentLibrary<const CharacterSubrace>& subraces,
    const Groups& groups, const StorageContentLibrary<const CharacterRace>& races
) noexcept
    : ContentFileParser(filepath), subraces(subraces), races(races), features_parser(type, filepath, groups) {}

} // namespace dnd

#endif // CHARACTER_SUBRACE_FILE_PARSER_HPP_
