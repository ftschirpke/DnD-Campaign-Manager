#ifndef CHARACTER_SUBCLASS_FILE_PARSER_HPP_
#define CHARACTER_SUBCLASS_FILE_PARSER_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "parsing/models/content_file_parser.hpp"

namespace dnd {

class CharacterSubclassFileParser : public ContentFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubclass>>& results;
    std::vector<std::shared_ptr<const Feature>> features;
public:
    CharacterSubclassFileParser(std::unordered_map<std::string, std::shared_ptr<const CharacterSubclass>>& results);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
    void reset() override;
};

inline CharacterSubclassFileParser::CharacterSubclassFileParser(
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubclass>>& results
)
    : results(results) {}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_FILE_PARSER_HPP_
