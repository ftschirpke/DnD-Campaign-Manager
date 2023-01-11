#include "dnd_config.hpp"

#include "spellcasting_feature_holder_file_parser.hpp"

#include <array>
#include <memory>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "models/spellcasting/preparation_spellcasting.hpp"
#include "models/spellcasting/spellcasting.hpp"
#include "models/spellcasting/spells_known_spellcasting.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::SpellcastingFeatureHolderFileParser::parseSize20Array(
    const nlohmann::json& json_to_parse, const char* attribute_name, std::array<int, 20>& output
) {
    const nlohmann::json& attribute_json = json_to_parse.at(attribute_name);
    if (!attribute_json.is_array() || attribute_json.size() != 20) {
        throw invalid_attribute(filepath, attribute_name, "should be an array of 20 numbers.");
    }
    output = attribute_json.get<std::array<int, 20>>();
}

void dnd::SpellcastingFeatureHolderFileParser::parseSpellcasting() {}

std::unique_ptr<dnd::Spellcasting> dnd::SpellcastingFeatureHolderFileParser::retrieveSpellcasting() {
    switch (spellcasting_type) {
        case PREPARATION:
            return std::make_unique<PreparationSpellcasting>(ability, ritual_casting, preparation_spellcasting_type);
        case SPELLS_KNOWN:
            return std::make_unique<SpellsKnownSpellcasting>(ability, ritual_casting);
        default:
            throw std::logic_error("Parsing of spellcasting type not implemented.");
    }
}
