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

class StringGroupsFileParser : public ContentFileParser {
public:
    StringGroupsFileParser(Groups& groups) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    static const ParsingType type;
    Groups& groups;
    std::unordered_map<std::string, std::unordered_set<std::string>> parsed_data;
    EffectHolderParser effect_holder_parser;
    std::unordered_set<std::string> parseMap(const nlohmann::json& json_map);
    virtual void configureSubparsers() override;
};

inline const ParsingType StringGroupsFileParser::type = ParsingType::GROUP;

inline StringGroupsFileParser::StringGroupsFileParser(Groups& groups) noexcept
    : ContentFileParser(), groups(groups), effect_holder_parser(groups) {}

inline void StringGroupsFileParser::configureSubparsers() { effect_holder_parser.configure(type, filepath); }

} // namespace dnd

#endif // STRING_GROUPS_FILE_PARSER_HPP_
