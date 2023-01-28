#ifndef CHARACTER_SUBCLASS_FILE_PARSER_HPP_
#define CHARACTER_SUBCLASS_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>

#include "controllers/groups.hpp"
#include "models/character_class.hpp"
#include "models/character_subclass.hpp"
#include "models/spell.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/features_parser.hpp"
#include "parsing/models/spellcasting/spellcasting_parser.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

class CharacterSubclassFileParser : public ContentFileParser {
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
    static const ParsingType type;
    std::unordered_map<std::string, const CharacterSubclass>& results;
    const std::unordered_map<std::string, const CharacterClass>& classes;
    std::string character_subclass_name, class_name;
    FeaturesParser features_parser;
    SpellcastingParser spellcasting_parser;
    virtual void configureSubparsers() override;
};

inline const ParsingType CharacterSubclassFileParser::type = ParsingType::SUBCLASS;

inline CharacterSubclassFileParser::CharacterSubclassFileParser(
    std::unordered_map<std::string, const CharacterSubclass>& results, const Groups& groups,
    const std::unordered_map<std::string, const CharacterClass>& classes,
    const std::unordered_map<std::string, const Spell>& spells
) noexcept
    : ContentFileParser(), results(results), classes(classes), features_parser(groups), spellcasting_parser(spells) {}

inline void CharacterSubclassFileParser::configureSubparsers() {
    features_parser.configure(type, filepath);
    spellcasting_parser.configure(type, filepath);
}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_FILE_PARSER_HPP_
