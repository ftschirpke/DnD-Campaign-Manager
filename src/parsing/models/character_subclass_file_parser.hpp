#ifndef CHARACTER_SUBCLASS_FILE_PARSER_HPP_
#define CHARACTER_SUBCLASS_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "models/effects_holder/feature.hpp"
#include "parsing/models/spellcasting_feature_holder_file_parser.hpp"

namespace dnd {

class CharacterSubclassFileParser : public SpellcastingFeatureHolderFileParser {
public:
    CharacterSubclassFileParser(
        std::unordered_map<std::string, const CharacterSubclass>& results,
        const std::unordered_map<std::string, const CharacterClass>& classes
    ) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
private:
    std::unordered_map<std::string, const CharacterSubclass>& results;
    const std::unordered_map<std::string, const CharacterClass>& classes;
    std::string character_subclass_name, class_name;
};

inline CharacterSubclassFileParser::CharacterSubclassFileParser(
    std::unordered_map<std::string, const CharacterSubclass>& results,
    const std::unordered_map<std::string, const CharacterClass>& classes
) noexcept
    : SpellcastingFeatureHolderFileParser(), results(results), classes(classes) {}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_FILE_PARSER_HPP_
