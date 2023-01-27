#include "dnd_config.hpp"

#include "character_race_file_parser.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#include <nlohmann/json.hpp>

#include "models/character_race.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterRaceFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::RACE, filepath, "map/object");
    }
    character_race_name = json_to_parse.at("name").get<std::string>();
    if (character_race_name.empty()) {
        throw invalid_attribute(ParsingType::RACE, filepath, "name", "cannot be \"\".");
    }
    has_subraces = json_to_parse.at("has_subraces").get<bool>();

    try {
        parseFeatures();
    } catch (parsing_error& e) {
        e.setParsingType(ParsingType::RACE);
        throw e;
    }
}

bool dnd::CharacterRaceFileParser::validate() const {
    if (results.contains(character_race_name)) {
        std::cerr << "Warning: Duplicate of race \"" << character_race_name << "\" found.\n";
        return false;
    }
    return true;
}

void dnd::CharacterRaceFileParser::saveResult() {
    results.emplace(
        std::piecewise_construct, std::forward_as_tuple(character_race_name),
        std::forward_as_tuple(character_race_name, retrieveFeatures(), has_subraces)
    );
}
