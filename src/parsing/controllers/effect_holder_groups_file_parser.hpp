#ifndef EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_
#define EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/choosable.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/effect_holder_parser.hpp"

namespace dnd {

/**
 * @brief A class for parsing multi-file groups i.e. choosable groups
 */
class EffectHolderGroupsFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs an EffectHolderGroupsFileParser
     * @param filepath the file to parse
     * @param groups the already-parsed groups
     */
    EffectHolderGroupsFileParser(const std::filesystem::path& filepath, Groups& groups) noexcept;
    /**
     * @brief Parses JSON file containing a choosable group
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed choosable group is valid
     * @return "true" if the group is valid, "false" otherwise
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed group to the groups.
     */
    virtual void saveResult() override;
    /**
     * @brief Returns the type of content that this parser parses - choosable groups
     * @return the type of content that this parser parses - choosable groups
     */
    virtual constexpr ParsingType getType() const override { return type; };
private:
    /**
     * @brief Parse and create a choosable
     * @param name the name of the choosable
     * @param choosable_json the body of the choosable definition
     * @return a choosable with the given name and the properties defined in the body
     * @throws parsing_error if any error occured while trying to parse the choosable_json
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    Choosable createChoosable(const std::string& name, const nlohmann::json& choosable_json) const;

    // the type of content that this parser parses - choosable groups
    static constexpr ParsingType type = ParsingType::GROUP;
    // the already-parsed groups where the result is added in the end
    Groups& groups;
    // the name of the parsed group
    std::string group_name;
    // the parsed choosables mapped to their respective names
    std::unordered_map<std::string, Choosable> choosables;
    // a subparser for effect holders used for parsing the effect holders for the choosable
    EffectHolderParser effect_holder_parser;
};

inline EffectHolderGroupsFileParser::EffectHolderGroupsFileParser(
    const std::filesystem::path& filepath, Groups& groups
) noexcept
    : ContentFileParser(filepath), groups(groups), effect_holder_parser(type, filepath, groups) {}

} // namespace dnd

#endif // EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_
