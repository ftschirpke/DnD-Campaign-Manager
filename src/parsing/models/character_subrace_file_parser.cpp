#include "dnd_config.hpp"

#include "character_subrace_file_parser.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "models/character_race.hpp"
#include "models/character_subrace.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterSubraceFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::SUBRACE, filepath, "map/object");
    }
    character_subrace_name = json_to_parse.at("name").get<std::string>();
    if (character_subrace_name.empty()) {
        throw invalid_attribute(ParsingType::SUBRACE, filepath, "name", "cannot be \"\".");
    }
    race_name = json_to_parse.at("race").get<std::string>();

    try {
        parseFeatures();
    } catch (parsing_error& e) {
        e.setParsingType(ParsingType::SUBRACE);
        throw e;
    }
}

bool dnd::CharacterSubraceFileParser::validate() const {
    if (results.find(character_subrace_name) != results.end()) {
        std::cerr << "Warning: Duplicate of subrace \"" << character_subrace_name << "\" found.\n";
        return false;
    }
    if (races.find(race_name) == races.cend()) {
        throw invalid_attribute(
            ParsingType::SUBRACE, filepath, "race", "must exist. \"" + race_name + "\" does not exist."
        );
    }
    if (!races.at(race_name).has_subraces) {
        throw invalid_attribute(
            ParsingType::SUBRACE, filepath, "race", "must have subraces. \"" + race_name + "\" does not have subraces."
        );
    }
    return true;
}

void dnd::CharacterSubraceFileParser::saveResult() {
    results.emplace(
        std::piecewise_construct, std::forward_as_tuple(character_subrace_name),
        std::forward_as_tuple(character_subrace_name, retrieveFeatures(), race_name)
    );
}
