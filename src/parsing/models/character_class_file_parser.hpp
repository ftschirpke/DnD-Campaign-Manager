#ifndef CHARACTER_CLASS_FILE_PARSER_HPP_
#define CHARACTER_CLASS_FILE_PARSER_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "parsing/models/content_file_parser.hpp"

namespace dnd {

class CharacterClassFileParser : public ContentFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results_map;
    std::string character_class_name, character_class_hit_dice;
    std::vector<int> asi_levels;
    std::vector<std::shared_ptr<const Feature>> features;
public:
    CharacterClassFileParser(std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results_map);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
    void reset() override;
};

inline CharacterClassFileParser::CharacterClassFileParser(
    std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results_map
)
    : results_map(results_map) {}

} // namespace dnd

#endif // CHARACTER_CLASS_FILE_PARSER_HPP_
