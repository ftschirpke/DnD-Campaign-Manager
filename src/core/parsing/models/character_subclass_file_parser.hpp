#ifndef CHARACTER_SUBCLASS_FILE_PARSER_HPP_
#define CHARACTER_SUBCLASS_FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/controllers/content_library.hpp>
#include <core/controllers/groups.hpp>
#include <core/models/character_class.hpp>
#include <core/models/character_subclass.hpp>
#include <core/models/spell.hpp>
#include <core/parsing/content_file_parser.hpp>
#include <core/parsing/models/effect_holder/features_parser.hpp>
#include <core/parsing/models/spellcasting/spellcasting_parser.hpp>
#include <core/parsing/subparser.hpp>

namespace dnd {

/**
 * @brief A class for parsing character subclasses (multi-file)
 */
class CharacterSubclassFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a CharacterSubclassFileParser
     * @param filepath the file to parse
     * @param subclasses the already-parsed subclasses
     * @param groups the already-parsed groups
     * @param classes the already-parsed classes
     * @param spells the already-parsed spells
     */
    CharacterSubclassFileParser(
        const std::filesystem::path& filepath, StorageContentLibrary<const CharacterSubclass>& subclasses,
        const Groups& groups, const StorageContentLibrary<const CharacterClass>& classes,
        const StorageContentLibrary<const Spell>& spells
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
    virtual void save_result() override;
    /**
     * @brief Returns the type of content that this parser parses - subclasses
     * @return the type of content that this parser parses - subclasses
     */
    virtual constexpr ParsingType get_type() const override { return type; };
private:
    // the type of content that this parser parses - subclasses
    static constexpr ParsingType type = ParsingType::SUBCLASS;
    // the name of the parsed subclass
    std::string character_subclass_name;
    // the name of the class for the parsed subclass
    std::string class_name;
    // the already-parsed subclasses to add the parsed subclass to
    StorageContentLibrary<const CharacterSubclass>& subclasses;
    // the already-parsed classes to check whether such a class exists
    const StorageContentLibrary<const CharacterClass>& classes;
    // a subparser used for parsing the subclass' features
    FeaturesParser features_parser;
    // a subparser used for parsing the subclass' spellcasting feature if it exists
    SpellcastingParser spellcasting_parser;
};

inline CharacterSubclassFileParser::CharacterSubclassFileParser(
    const std::filesystem::path& filepath, StorageContentLibrary<const CharacterSubclass>& subclasses,
    const Groups& groups, const StorageContentLibrary<const CharacterClass>& classes,
    const StorageContentLibrary<const Spell>& spells
) noexcept
    : ContentFileParser(filepath), subclasses(subclasses), classes(classes), features_parser(type, filepath, groups),
      spellcasting_parser(type, filepath, spells) {}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_FILE_PARSER_HPP_
