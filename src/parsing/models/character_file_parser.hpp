#ifndef CHARACTER_FILE_PARSER_HPP_
#define CHARACTER_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_subclass.hpp"
#include "models/character_subrace.hpp"
#include "models/effect_holder/character_decision.hpp"
#include "models/spell.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/effect_holder_parser.hpp"
#include "parsing/models/effect_holder/features_parser.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

/**
 * @brief A class for parsing characters (multi-file)
 */
class CharacterFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a CharacterFileParser
     * @param filepath the file to parse
     * @param characters the already-parsed characters
     * @param groups the already-parsed groups
     * @param character_classes the already-parsed classes
     * @param character_subclasses the already-parsed subclasses
     * @param character_races the already-parsed races
     * @param character_subraces the already-parsed subraces
     * @param spells the already-parsed spells
     */
    CharacterFileParser(
        const std::filesystem::path& filepath, std::unordered_map<std::string, Character>& characters,
        const Groups& groups, const std::unordered_map<std::string, const CharacterClass>& character_classes,
        const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses,
        const std::unordered_map<std::string, const CharacterRace>& character_races,
        const std::unordered_map<std::string, const CharacterSubrace>& character_subraces,
        const std::unordered_map<std::string, const Spell>& spells
    ) noexcept;
    /**
     * @brief Parses JSON file containing a character
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed character is valid
     * @return "true" if the character is valid, "false" otherwise
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed character
     */
    virtual void saveResult() override;
    /**
     * @brief Returns the type of content that this parser parses - characters
     * @return the type of content that this parser parses - characters
     */
    virtual constexpr ParsingType getType() const override { return type; };
protected:
    /**
     * @brief Parse the character decision for a choice required by a particular feature-like object
     * @param feature_name the name of the feature-like object
     * @param feature_decisions_json the JSON containing the decisions for that feature-like object
     * @throws parsing_error if any error occured while trying to parse the feature_decisions_json
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    void parseCharacterDecisions(const std::string& feature_name, const nlohmann::json& feature_decisions_json);
private:
    /**
     * @brief Parses the class, race, subclass, and subrace of the character
     * @throws parsing_error if the chosen class, race, subclass, or subrace is invalid
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    void parseClassAndRace();
    /**
     * @brief Parses the level and XP values of the character
     * And if only one of them was provided the other value is determined from the first
     * @throws parsing_error if the chosen level or XP value is invalid
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    void parseLevelAndXP();

    // the type of content that this parser parses - characters
    static constexpr ParsingType type = ParsingType::CHARACTER;
    // the name of the parsed character
    std::string character_name;
    // the base ability scores of the parsed character
    std::array<int, 6> base_ability_scores;
    // the hit dice rolls of the parsed character
    std::vector<int> hit_dice_rolls;
    // a pointer to the class of the parsed character
    const CharacterClass* class_ptr;
    // a pointer to the subclass of the parsed character
    const CharacterSubclass* subclass_ptr;
    // a pointer to the race of the parsed character
    const CharacterRace* race_ptr;
    // a pointer to the subrace of the parsed character
    const CharacterSubrace* subrace_ptr;
    // the decisions of the parsed character
    std::vector<CharacterDecision> decisions;
    // the level of the parsed character
    int level;
    // the XP value of the parsed character
    int xp;
    // the already-parsed characters to add the parsed character to
    std::unordered_map<std::string, Character>& characters;
    // the already-parsed classes to find the parsed character's class
    const std::unordered_map<std::string, const CharacterClass>& character_classes;
    // the already-parsed classes to find the parsed character's subclass
    const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses;
    // the already-parsed classes to find the parsed character's race
    const std::unordered_map<std::string, const CharacterRace>& character_races;
    // the already-parsed classes to find the parsed character's subrace
    const std::unordered_map<std::string, const CharacterSubrace>& character_subraces;
    // the already-parsed spells
    const std::unordered_map<std::string, const Spell>& spells;
    // a subparser for effect holders used for parsing the effect holders for the character decisions
    EffectHolderParser effect_holder_parser;
    // a subparser used for parsing the character's features
    FeaturesParser features_parser;
};

inline CharacterFileParser::CharacterFileParser(
    const std::filesystem::path& filepath, std::unordered_map<std::string, Character>& characters, const Groups& groups,
    const std::unordered_map<std::string, const CharacterClass>& character_classes,
    const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses,
    const std::unordered_map<std::string, const CharacterRace>& character_races,
    const std::unordered_map<std::string, const CharacterSubrace>& character_subraces,
    const std::unordered_map<std::string, const Spell>& spells
) noexcept
    : ContentFileParser(filepath), class_ptr(nullptr), subclass_ptr(nullptr), race_ptr(nullptr), subrace_ptr(nullptr),
      characters(characters), character_classes(character_classes), character_subclasses(character_subclasses),
      character_races(character_races), character_subraces(character_subraces), spells(spells),
      effect_holder_parser(type, filepath, groups), features_parser(type, filepath, groups) {}

} // namespace dnd

#endif // CHARACTER_FILE_PARSER_HPP_
