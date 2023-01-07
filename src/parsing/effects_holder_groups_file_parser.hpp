#ifndef EFFECTS_HOLDER_GROUPS_FILE_PARSER_HPP_
#define EFFECTS_HOLDER_GROUPS_FILE_PARSER_HPP_

#include "controllers/groups.hpp"
#include "parsing/models/effects_holder_file_parser.hpp"

namespace dnd {

class EffectsHolderGroupsFileParser : public EffectsHolderFileParser {
public:
    EffectsHolderGroupsFileParser(Groups& results);
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    Groups& results;
};

inline EffectsHolderGroupsFileParser::EffectsHolderGroupsFileParser(Groups& results) : results(results) {}

} // namespace dnd

#endif // EFFECTS_HOLDER_GROUPS_FILE_PARSER_HPP_
