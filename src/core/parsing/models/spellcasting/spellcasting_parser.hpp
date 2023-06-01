#ifndef SPELLCASTING_PARSER_HPP_
#define SPELLCASTING_PARSER_HPP_

#include "dnd_config.hpp"

#include <array>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "core/controllers/content_library.hpp"
#include "core/models/spell.hpp"
#include "core/models/spellcasting/preparation_spellcasting.hpp"
#include "core/models/spellcasting/spellcasting.hpp"
#include "core/models/spellcasting/spells_known_spellcasting.hpp"
#include "core/parsing/subparser.hpp"

namespace dnd {

/**
 * @brief An enum for the types of spellcasting
 */
enum SpellcastingType {
    PREPARATION,
    SPELLS_KNOWN,
};

/**
 * @brief A subparser for parsing spellcasting features of a class or subclass
 */
class SpellcastingParser : public Subparser {
public:
    /**
     * @brief Constructs a SpellcastingParser
     * @param spells the already-parsed spells
     */
    SpellcastingParser(
        ParsingType type, const std::filesystem::path& filepath, const StoringContentLibrary<const Spell>& spells
    ) noexcept;
    /**
     * @brief Parses the spellcasting feature from a given JSON
     * @param spellcasting_json the JSON that needs to be parsed
     * @throws parsing_error if any error occured while trying to parse the spellcasting_json
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    void parseSpellcasting(const nlohmann::json& spellcasting_json);
    /**
     * @brief Creates and returns the parsed spellcasting feature while giving up ownership over all the parsed values
     * @return a unique pointer to the parsed spellcasting
     * @throws attribute_type_error if the creation of this spellcasting type is not implemented
     */
    std::unique_ptr<Spellcasting> retrieveSpellcasting();
private:
    /**
     * @brief A helper function to parse optional attributes of which the value should be an length-20 integer array
     * @param json_to_parse the JSON the optional attribute should be in, if it exists
     * @param attribute_name the name of the attribute
     * @param output the output to write the value of the attribute to if it exists or an array full of zeros otherwise
     * @throws invalid_attribute if the attribute's value is not an array of size 20
     * @throws nlohmann::json::type_error if the array entries aren't integers
     */
    void parseSize20Array(const nlohmann::json& json_to_parse, const char* attribute_name, std::array<int, 20>& output);

    // the already-parsed spells to look up spell lists in
    const StoringContentLibrary<const Spell>& spells;
    // the parsed spellcasting ability
    std::string ability;
    // the parsed ritual_casting value for the spellcasting
    bool ritual_casting;
    // the parsed spell list for the spellcasting
    std::unordered_map<std::string, const Spell*> spell_list;
    // the type of the spellcasting
    SpellcastingType spellcasting_type;
    // the type of preparation spellcasting if it is a preparation spellcasting
    PreparationSpellcastingType preparation_spellcasting_type;
    // the parsed array of spells known values for the spellcasting
    std::array<int, 20> spells_known;
    // the parsed array of cantrips known values for the spellcasting
    std::array<int, 20> cantrips_known;
    // the parsed arrays of spell slots for the spellcasting
    std::array<std::array<int, 20>, 9> spell_slots;
};

inline SpellcastingParser::SpellcastingParser(
    ParsingType type, const std::filesystem::path& filepath, const StoringContentLibrary<const Spell>& spells
) noexcept
    : Subparser(type, filepath), spells(spells) {}

} // namespace dnd

#endif // SPELLCASTING_PARSER_HPP_
