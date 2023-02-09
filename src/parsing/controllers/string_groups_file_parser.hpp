#ifndef STRING_GROUPS_FILE_PARSER_HPP_
#define STRING_GROUPS_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/effect_holder_parser.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

/**
 * @brief A class for parsing single-file groups i.e. string groups
 */
class StringGroupsFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs an StringGroupsFileParser
     * @param groups the already-parsed groups
     */
    StringGroupsFileParser(Groups& groups) noexcept;
    /**
     * @brief Parses JSON file containing a string group
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed string group is valid
     * @return "true" if the group is valid, "false" otherwise
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed group to the groups.
     */
    virtual void saveResult() override;
private:
    /**
     * @brief Parses one level of the input-map and saves the results in parsed_data
     * @param json_map the part of the JSON map that should be parsed
     * @return any values parsed as "__no_subgroup__" meaning they belong to the level above
     * @throws attribute_format_error if the map's format is invalid
     */
    std::unordered_set<std::string> parseMap(const nlohmann::json& json_map);
    /**
     * @brief Configures the subparsers used
     */
    virtual void configureSubparsers() override;

    // the type of content that this parser parses - string groups
    static constexpr ParsingType type = ParsingType::GROUP;
    // the already-parsed groups where the result is added in the end
    Groups& groups;
    // the parsed groups
    std::unordered_map<std::string, std::unordered_set<std::string>> parsed_data;
    // a subparser for effect holders used for parsing the effect holders for the choosable
    EffectHolderParser effect_holder_parser;
};

inline StringGroupsFileParser::StringGroupsFileParser(Groups& groups) noexcept
    : ContentFileParser(), groups(groups), effect_holder_parser(groups) {}

inline void StringGroupsFileParser::configureSubparsers() { effect_holder_parser.configure(type, filepath); }

} // namespace dnd

#endif // STRING_GROUPS_FILE_PARSER_HPP_
