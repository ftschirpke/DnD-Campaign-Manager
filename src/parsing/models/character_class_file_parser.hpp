#ifndef CHARACTER_CLASS_FILE_PARSER_HPP_
#define CHARACTER_CLASS_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "models/effects_holder/feature.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

class CharacterClassFileParser : public FeatureHolderFileParser {
public:
    CharacterClassFileParser(std::unordered_map<std::string, const CharacterClass>& results) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
protected:
    int determineSubclassLevel(const std::vector<Feature>& features) const;
private:
    std::unordered_map<std::string, const CharacterClass>& results;
    std::string character_class_name, character_class_hit_dice;
    std::vector<int> asi_levels;
    int subclass_level;
};

inline CharacterClassFileParser::CharacterClassFileParser(std::unordered_map<std::string, const CharacterClass>& results
) noexcept
    : FeatureHolderFileParser(), results(results) {}

} // namespace dnd

#endif // CHARACTER_CLASS_FILE_PARSER_HPP_
