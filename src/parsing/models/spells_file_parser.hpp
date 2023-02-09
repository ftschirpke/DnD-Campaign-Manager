#ifndef SPELLS_FILE_PARSER_HPP_
#define SPELLS_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/spell.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/parsing_types.hpp"

namespace dnd {

/**
 * @brief A struct to save a spell's information during parsing
 */
struct SpellParsingInfo {
    // the name of the spell
    std::string name;
    // a description of how long the spell takes to cast
    std::string casting_time;
    // a description of the range of the spell
    std::string range;
    // a description of how long the effect of the spell lasts
    std::string duration;
    // a full description of the spell
    std::string description;
    // the names of classes (and subclasses) that can cast the spell
    std::unordered_set<std::string> classes;
    // the type of the spell
    SpellType type;
    // the components required to cast the spell
    SpellComponents components;
};

/**
 * @brief A class for parsing spells (multi-file)
 */
class SpellsFileParser : public ContentFileParser {
public:
    /**
     * @brief Constructs a SpellsFileParser
     * @param spells the already-parsed spells
     * @param groups the already-parsed groups
     */
    SpellsFileParser(std::unordered_map<std::string, const Spell>& spells, Groups& groups) noexcept;
    /**
     * @brief Parses JSON file containing a collection of spells
     * @throws parsing_error if any error occured while trying to parse the content file
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    virtual void parse() override;
    /**
     * @brief Checks whether the parsed spells are valid
     * @return "true" if the spells are valid, "false" otherwise
     */
    virtual bool validate() const override;
    /**
     * @brief Saves the parsed spells
     */
    virtual void saveResult() override;
protected:
    /**
     * @brief Parses a spell and saves its information
     * @param spell_name the name of the spell
     * @param spell_json the JSON containing the spell's information
     * @throws parsing_error if any error occured while trying to parse the spell_json
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    void createSpell(std::string_view spell_name, const nlohmann::json& spell_json);
    /**
     * @brief Parses and creates a spell type
     * @param spell_type_str the string to parse
     * @return the spell type parsed from the string
     * @throws attribute_type_error if spell_type_str does not define a valid spell type
     */
    SpellType createSpellType(const std::string& spell_type_str) const;
    /**
     * @brief Parses and creates a spell components object
     * @param spell_components_str the string to parse
     * @return the spell components object parsed from the string
     * @throws attribute_type_error if spell_components_str does not define valid spell components
     */
    SpellComponents createSpellComponents(const std::string& spell_components_str) const;
private:
    /**
     * @brief Configures the subparsers used
     */
    virtual void configureSubparsers() override;

    // the type of content that this parser parses - spells
    static constexpr ParsingType type = ParsingType::SPELL;
    // the c-style string to create the regular expression to check the validity of spell components from
    static const char* const spell_components_regex_cstr;
    // the c-style string to create the regular expression to check the validity of a spell type as c-style string from
    static const char* const spell_type_regex_cstr;
    // the regular expression to check the validity of spell components
    const std::regex spell_components_regex;
    // the regular expression to check the validity of a spell type
    const std::regex spell_type_regex;
    // the already-parsed spells to add the parsed spells to
    std::unordered_map<std::string, const Spell>& spells;
    // the already-parsed groups to add spell-groups to
    Groups& groups;
    // the amount of spells to be parsed in the current file
    size_t spells_in_file;
    // the spell information already parsed for the current file
    std::vector<SpellParsingInfo> spell_parsing_info;
    // a vector to keep track of which spells in this file are valid
    mutable std::vector<bool> valid;
    // a mutex to control writing access to the spell_parsing_info vector
    std::mutex spell_parsing_mutex;
};

inline SpellsFileParser::SpellsFileParser(std::unordered_map<std::string, const Spell>& spells, Groups& groups) noexcept
    : ContentFileParser(), spell_components_regex(spell_type_regex_cstr), spell_type_regex(spell_components_regex_cstr),
      spells(spells), groups(groups) {}

inline void SpellsFileParser::configureSubparsers() {} // SpellsFileParser has no subparsers

} // namespace dnd

#endif // SPELLS_FILE_PARSER_HPP_
