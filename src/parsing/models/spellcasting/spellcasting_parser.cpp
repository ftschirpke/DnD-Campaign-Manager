#include "dnd_config.hpp"

#include "spellcasting_parser.hpp"

#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <nlohmann/json.hpp>

#include "basic_mechanics/abilities.hpp"
#include "models/spellcasting/preparation_spellcasting.hpp"
#include "models/spellcasting/spellcasting.hpp"
#include "models/spellcasting/spells_known_spellcasting.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"
#include "parsing/subparser.hpp"

void dnd::SpellcastingParser::parseSize20Array(
    const nlohmann::json& json_to_parse, const char* attribute_name, std::array<int, 20>& output
) {
    requiresConfiguration();

    if (!json_to_parse.contains(attribute_name)) {
        output = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        return;
    }
    const nlohmann::json& attribute_json = json_to_parse.at(attribute_name);
    if (!attribute_json.is_array() || attribute_json.size() != 20) {
        throw invalid_attribute(type, filepath, attribute_name, "should be an array of 20 numbers.");
    }
    output = attribute_json.get<std::array<int, 20>>();
}

void dnd::SpellcastingParser::parseSpellcasting(const nlohmann::json& spellcasting_json) {
    DND_MEASURE_FUNCTION();

    requiresConfiguration();

    ability = spellcasting_json.at("ability").get<std::string>();
    if (!isAbility(ability)) {
        throw invalid_attribute(type, filepath, "spellcasting:attribute", "is not an ability");
    }
    ritual_casting = spellcasting_json.at("ritual_casting").get<bool>();

    bool has_spells_known = spellcasting_json.contains("spells_known");
    bool has_preparation_caster = spellcasting_json.contains("preparation_caster");
    if (has_spells_known && has_preparation_caster) {
        throw invalid_attribute(
            type, filepath, "spellcasting:spells_known/preparation_caster", "attributes are mutally exclusive."
        );
    } else if (has_spells_known) {
        spellcasting_type = SpellcastingType::SPELLS_KNOWN;
        parseSize20Array(spellcasting_json, "spells_known", spells_known);
    } else if (has_preparation_caster) {
        spellcasting_type = SpellcastingType::PREPARATION;
        std::string preparation_spellcasting_type_str = spellcasting_json.at("preparation_caster").get<std::string>();
        if (preparation_spellcasting_type_str == "half") {
            preparation_spellcasting_type = PreparationSpellcastingType::HALF;
        } else if (preparation_spellcasting_type_str == "full") {
            preparation_spellcasting_type = PreparationSpellcastingType::HALF;
        } else {
            throw invalid_attribute(type, filepath, "spellcasting:preparation_caster", "must be \"half\" or \"full\".");
        }
    } else {
        throw attribute_missing(type, filepath, "spellcasting:spells_known or spellcasting:preparation_caster");
    }

    parseSize20Array(spellcasting_json, "cantrips_known", cantrips_known);
    parseSize20Array(spellcasting_json, "level1_slots", spell_slots[0]);
    parseSize20Array(spellcasting_json, "level2_slots", spell_slots[1]);
    parseSize20Array(spellcasting_json, "level3_slots", spell_slots[2]);
    parseSize20Array(spellcasting_json, "level4_slots", spell_slots[3]);
    parseSize20Array(spellcasting_json, "level5_slots", spell_slots[4]);
    parseSize20Array(spellcasting_json, "level6_slots", spell_slots[5]);
    parseSize20Array(spellcasting_json, "level7_slots", spell_slots[6]);
    parseSize20Array(spellcasting_json, "level8_slots", spell_slots[7]);
    parseSize20Array(spellcasting_json, "level9_slots", spell_slots[8]);

    // TODO: parse spell list
    UNUSED(spells);
}

std::unique_ptr<dnd::Spellcasting> dnd::SpellcastingParser::retrieveSpellcasting() {
    std::unique_ptr<dnd::Spellcasting> spellcasting_ptr;
    switch (spellcasting_type) {
        case SpellcastingType::PREPARATION:
            spellcasting_ptr = std::make_unique<PreparationSpellcasting>(
                ability, ritual_casting, preparation_spellcasting_type
            );
            break;
        case SpellcastingType::SPELLS_KNOWN:
            spellcasting_ptr = std::make_unique<SpellsKnownSpellcasting>(ability, ritual_casting);
            break;
        default:
            throw std::logic_error("Parsing of spellcasting type not implemented.");
    }
    spellcasting_ptr->spell_list = std::move(spell_list);
    // TODO: add cantrips_known, spell_slots (and spells_known)
    return spellcasting_ptr;
}
