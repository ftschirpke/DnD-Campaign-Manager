#ifndef CHARACTER_SUBCLASS_FILE_PARSER_HPP_
#define CHARACTER_SUBCLASS_FILE_PARSER_HPP_

#include <string>
#include <unordered_map>

#include "controllers/groups.hpp"
#include "models/character_class.hpp"
#include "models/spell.hpp"
#include "parsing/models/spellcasting_feature_holder_file_parser.hpp"

namespace dnd {

class CharacterSubclassFileParser : public SpellcastingFeatureHolderFileParser {
public:
    CharacterSubclassFileParser(
        std::unordered_map<std::string, const CharacterSubclass>& results, const Groups& groups,
        const std::unordered_map<std::string, const CharacterClass>& classes,
        const std::unordered_map<std::string, const Spell>& spells
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
    std::unordered_map<std::string, const CharacterSubclass>& results, const Groups& groups,
    const std::unordered_map<std::string, const CharacterClass>& classes,
    const std::unordered_map<std::string, const Spell>& spells
) noexcept
    : SpellcastingFeatureHolderFileParser(spells, groups), results(results), classes(classes) {}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_FILE_PARSER_HPP_
