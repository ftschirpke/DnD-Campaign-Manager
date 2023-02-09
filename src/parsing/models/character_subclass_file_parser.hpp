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

/**
 * @brief A class for parsing character subclasses (multi-file)
 */
class CharacterSubclassFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a CharacterSubclassFileParser
     * @param subclasses the already-parsed subclasses
     * @param groups the already-parsed groups
     * @param classes the already-parsed classes
     * @param spells the already-parsed spells
     */
    CharacterSubclassFileParser(
        std::unordered_map<std::string, const CharacterSubclass>& subclasses, const Groups& groups,
        const std::unordered_map<std::string, const CharacterClass>& classes,
        const std::unordered_map<std::string, const Spell>& spells
    ) noexcept;
    /**
     * @brief Parses JSON file containing a subclass
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed subclass is valid
     * @return "true" if the subclass is valid, "false" otherwise
     * @throws invalid_attribute if the chosen corresponding class does not exist
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed subclass
     */
    virtual void saveResult() override;
private:
    /**
     * @brief Configures the subparsers used
     */
    virtual void configureSubparsers() override;

    // the type of content that this parser parses - subclasses
    static constexpr ParsingType type = ParsingType::SUBCLASS;
    // the name of the parsed subclass
    std::string character_subclass_name;
    // the name of the class for the parsed subclass
    std::string class_name;
    // the already-parsed subclasses to add the parsed subclass to
    std::unordered_map<std::string, const CharacterSubclass>& subclasses;
    // the already-parsed classes to check whether such a class exists
    const std::unordered_map<std::string, const CharacterClass>& classes;
    // a subparser used for parsing the subclass' features
    FeaturesParser features_parser;
    // a subparser used for parsing the subclass' spellcasting feature if it exists
    SpellcastingParser spellcasting_parser;
};

inline CharacterSubclassFileParser::CharacterSubclassFileParser(
    std::unordered_map<std::string, const CharacterSubclass>& subclasses, const Groups& groups,
    const std::unordered_map<std::string, const CharacterClass>& classes,
    const std::unordered_map<std::string, const Spell>& spells
) noexcept
    : ContentFileParser(), subclasses(subclasses), classes(classes), features_parser(groups),
      spellcasting_parser(spells) {}

inline void CharacterSubclassFileParser::configureSubparsers() {
    features_parser.configure(type, filepath);
    spellcasting_parser.configure(type, filepath);
}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_FILE_PARSER_HPP_
