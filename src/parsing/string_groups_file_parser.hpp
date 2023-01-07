#ifndef STRING_GROUPS_FILE_PARSER_HPP_
#define STRING_GROUPS_FILE_PARSER_HPP_

#include "controllers/groups.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

class StringGroupsFileParser : public ContentFileParser {
public:
    StringGroupsFileParser(Groups& results);
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    Groups& results;
};

inline StringGroupsFileParser::StringGroupsFileParser(Groups& results) : results(results) {}

} // namespace dnd

#endif // STRING_GROUPS_FILE_PARSER_HPP_
