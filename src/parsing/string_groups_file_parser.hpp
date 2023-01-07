#ifndef STRING_GROUPS_FILE_PARSER_HPP_
#define STRING_GROUPS_FILE_PARSER_HPP_

#include <unordered_map>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

class StringGroupsFileParser : public ContentFileParser {
public:
    StringGroupsFileParser(Groups& results);
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
protected:
    std::unordered_set<std::string> parseMap(const nlohmann::json& json_map);
private:
    Groups& results;
    std::unordered_map<std::string, std::unordered_set<std::string>> parsed_data;
};

inline StringGroupsFileParser::StringGroupsFileParser(Groups& results) : results(results) {}

} // namespace dnd

#endif // STRING_GROUPS_FILE_PARSER_HPP_
