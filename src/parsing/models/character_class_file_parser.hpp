#ifndef CHARACTER_CLASS_FILE_PARSER_HPP_
#define CHARACTER_CLASS_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "basic_mechanics/dice.hpp"
#include "controllers/groups.hpp"
#include "models/character_class.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/spell.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/features_parser.hpp"
#include "parsing/models/spellcasting/spellcasting_parser.hpp"
#include "parsing/parsing_types.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

class CharacterClassFileParser : public ContentFileParser {
public:
    CharacterClassFileParser(
        std::unordered_map<std::string, const CharacterClass>& classes, const Groups& groups,
        const std::unordered_map<std::string, const Spell>& spells
    ) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
protected:
    void determineSubclassLevel(const std::vector<Feature>& features);
private:
    static const ParsingType type;
    std::unordered_map<std::string, const CharacterClass>& classes;
    std::string character_class_name;
    Dice character_class_hit_dice;
    std::vector<int> asi_levels;
    int subclass_level;
    FeaturesParser features_parser;
    SpellcastingParser spellcasting_parser;
    virtual void configureSubparsers() override;
};

inline const ParsingType CharacterClassFileParser::type = ParsingType::CLASS;

inline CharacterClassFileParser::CharacterClassFileParser(
    std::unordered_map<std::string, const CharacterClass>& classes, const Groups& groups,
    const std::unordered_map<std::string, const Spell>& spells
) noexcept
    : ContentFileParser(), classes(classes), features_parser(groups), spellcasting_parser(spells) {}

inline void CharacterClassFileParser::configureSubparsers() {
    features_parser.configure(type, filepath);
    spellcasting_parser.configure(type, filepath);
}

} // namespace dnd

#endif // CHARACTER_CLASS_FILE_PARSER_HPP_
