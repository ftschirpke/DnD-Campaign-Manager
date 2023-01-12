#ifndef EFFECTS_HOLDER_GROUPS_FILE_PARSER_HPP_
#define EFFECTS_HOLDER_GROUPS_FILE_PARSER_HPP_

#include "controllers/groups.hpp"
#include "models/effects_holder/choosable.hpp"
#include "parsing/models/effects_holder_file_parser.hpp"

namespace dnd {

class EffectsHolderGroupsFileParser : public EffectsHolderFileParser {
public:
    EffectsHolderGroupsFileParser(Groups& results) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    Groups& results;
    std::string group_name;
    std::unordered_map<std::string, Choosable> choosables;
    Choosable createChoosable(const std::string& name, const nlohmann::json& choosable_json) const;
};

inline EffectsHolderGroupsFileParser::EffectsHolderGroupsFileParser(Groups& results) noexcept : results(results) {}

} // namespace dnd

#endif // EFFECTS_HOLDER_GROUPS_FILE_PARSER_HPP_
