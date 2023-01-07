#ifndef FEATURE_GROUPS_FILE_PARSER_HPP_
#define FEATURE_GROUPS_FILE_PARSER_HPP_

#include "controllers/groups.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

class FeatureGroupsFileParser : public ContentFileParser {
public:
    FeatureGroupsFileParser(Groups& results);
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    Groups& results;
};

inline FeatureGroupsFileParser::FeatureGroupsFileParser(Groups& results) : results(results) {}

} // namespace dnd

#endif // FEATURE_GROUPS_FILE_PARSER_HPP_
