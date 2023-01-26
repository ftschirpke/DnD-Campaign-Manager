#ifndef EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_
#define EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_

#include "controllers/groups.hpp"
#include "models/effect_holder/choosable.hpp"
#include "parsing/models/effect_holder_file_parser.hpp"

namespace dnd {

class EffectHolderGroupsFileParser : public EffectHolderFileParser {
public:
    EffectHolderGroupsFileParser(Groups& results) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    Groups& results;
    std::string group_name;
    std::unordered_map<std::string, Choosable> choosables;
    Choosable createChoosable(const std::string& name, const nlohmann::json& choosable_json) const;
};

inline EffectHolderGroupsFileParser::EffectHolderGroupsFileParser(Groups& results) noexcept
    : EffectHolderFileParser(results), results(results) {}

} // namespace dnd

#endif // EFFECT_HOLDER_GROUPS_FILE_PARSER_HPP_
