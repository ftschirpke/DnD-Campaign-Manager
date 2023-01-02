#ifndef CHARACTER_CLASS_FILE_PARSER_HPP_
#define CHARACTER_CLASS_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "models/features/feature.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

class CharacterClassFileParser : public FeatureHolderFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results;
    std::string character_class_name, character_class_hit_dice;
    std::vector<int> asi_levels;
    int subclass_level;
public:
    CharacterClassFileParser(std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
};

inline CharacterClassFileParser::CharacterClassFileParser(
    std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results
)
    : results(results) {}

} // namespace dnd

#endif // CHARACTER_CLASS_FILE_PARSER_HPP_
