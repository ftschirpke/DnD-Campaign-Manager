#ifndef CHARACTER_CLASS_FILE_PARSER_HPP_
#define CHARACTER_CLASS_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "core/basic_mechanics/dice.hpp"
#include "core/controllers/groups.hpp"
#include "core/models/character_class.hpp"
#include "core/models/effect_holder/feature.hpp"
#include "core/models/spell.hpp"
#include "core/parsing/content_file_parser.hpp"
#include "core/parsing/models/effect_holder/features_parser.hpp"
#include "core/parsing/models/spellcasting/spellcasting_parser.hpp"
#include "core/parsing/parsing_types.hpp"
#include "core/parsing/subparser.hpp"

namespace dnd {

/**
 * @brief A class for parsing character classes (multi-file)
 */
class CharacterClassFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a CharacterClassFileParser
     * @param filepath the file to parse
     * @param classes the already-parsed classes
     * @param groups the already-parsed groups
     * @param spells the already-parsed spells
     */
    CharacterClassFileParser(
        const std::filesystem::path& filepath, StoringContentLibrary<const CharacterClass>& classes, const Groups& groups,
        const StoringContentLibrary<const Spell>& spells
    ) noexcept;
    /**
     * @brief Parses JSON file containing a class
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
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
    /**
     * @brief Returns the type of content that this parser parses - classes
     * @return the type of content that this parser parses - classes
     */
    virtual constexpr ParsingType getType() const override { return type; };
protected:
    /**
     * @brief Determine the subclass level for the parsed class
     * @param features the classes' features of which one should be a subclass feature
     * @throws invalid_attribute if class does not have exactly one subclass feature or the subclass level is invalid
     */
    void determineSubclassLevel(const std::vector<Feature>& features);
private:
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
    StoringContentLibrary<const CharacterClass>& classes;
    // a subparser used for parsing the class' features
    FeaturesParser features_parser;
    // a subparser used for parsing the subclass' spellcasting feature if it exists
    SpellcastingParser spellcasting_parser;
};

inline CharacterClassFileParser::CharacterClassFileParser(
    const std::filesystem::path& filepath, StoringContentLibrary<const CharacterClass>& classes, const Groups& groups,
    const StoringContentLibrary<const Spell>& spells
) noexcept
    : ContentFileParser(filepath), classes(classes), features_parser(type, filepath, groups),
      spellcasting_parser(type, filepath, spells) {}

} // namespace dnd

#endif // CHARACTER_CLASS_FILE_PARSER_HPP_
