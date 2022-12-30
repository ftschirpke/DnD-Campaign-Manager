#ifndef CHARACTER_FILE_PARSER_HPP_
#define CHARACTER_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "models/character.hpp"
#include "parsing/models/content_file_parser.hpp"

namespace dnd {

class CharacterFileParser : public ContentFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const Character>>& results;
public:
    CharacterFileParser(std::unordered_map<std::string, std::shared_ptr<const Character>>& results);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
    void reset() override;
};

inline CharacterFileParser::CharacterFileParser(
    std::unordered_map<std::string, std::shared_ptr<const Character>>& results
)
    : results(results) {}

} // namespace dnd

#endif // CHARACTER_FILE_PARSER_HPP_
