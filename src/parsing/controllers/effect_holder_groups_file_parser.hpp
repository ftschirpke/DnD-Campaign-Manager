#ifndef EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_
#define EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/choosable.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/effect_holder_parser.hpp"

namespace dnd {

class EffectHolderGroupsFileParser : public ContentFileParser {
public:
    EffectHolderGroupsFileParser(Groups& groups) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    static const ParsingType type;
    Groups& groups;
    std::string group_name;
    std::unordered_map<std::string, Choosable> choosables;
    EffectHolderParser effect_holder_parser;
    Choosable createChoosable(const std::string& name, const nlohmann::json& choosable_json) const;
    virtual void configureSubparsers() override;
};

inline const ParsingType EffectHolderGroupsFileParser::type = ParsingType::GROUP;

inline EffectHolderGroupsFileParser::EffectHolderGroupsFileParser(Groups& groups) noexcept
    : ContentFileParser(), groups(groups), effect_holder_parser(groups) {}

inline void EffectHolderGroupsFileParser::configureSubparsers() { effect_holder_parser.configure(type, filepath); }

} // namespace dnd

#endif // EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_
