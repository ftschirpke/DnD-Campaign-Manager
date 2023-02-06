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

/**
 * @brief A class for parsing character classes (multi-file)
 */
class CharacterClassFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a CharacterClassFileParser
     * @param classes the already-parsed classes
     * @param groups the already-parsed groups
     * @param spells the already-parsed spells
     */
    CharacterClassFileParser(
        std::unordered_map<std::string, const CharacterClass>& classes, const Groups& groups,
        const std::unordered_map<std::string, const Spell>& spells
    ) noexcept;
    /**
     * @brief Parses JSON file containing a class.
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed class is valid
     * @return "true" if the class is valid, "false" otherwise
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed class
     */
    virtual void saveResult() override;
protected:
    /**
     * @brief Determine the subclass level for the parsed class
     * @param features the classes' features of which one should be a subclass feature
     */
    void determineSubclassLevel(const std::vector<Feature>& features);
private:
    /**
     * @brief Configures the subparsers used
     */
    virtual void configureSubparsers() override;

    // the type of content that this parser parses - classes
    static constexpr ParsingType type = ParsingType::CLASS;
    // the name of the parsed class
    std::string character_class_name;
    // the hit dice of the parsed class
    Dice character_class_hit_dice;
    // the levels the parsed class allows ability score increases
    std::vector<int> asi_levels;
    // the subclass level of the parsed class
    int subclass_level;
    // the already-parsed classes to add the parsed class to
    std::unordered_map<std::string, const CharacterClass>& classes;
    // a subparser used for parsing the class' features
    FeaturesParser features_parser;
    // a subparser used for parsing the subclass' spellcasting feature if it exists
    SpellcastingParser spellcasting_parser;
};

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
