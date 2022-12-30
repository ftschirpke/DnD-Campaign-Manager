#include "character_subrace_file_parser.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_race.hpp"
#include "parsing/models/features/feature_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterSubraceFileParser::parse() {
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::SUBRACES, filename, "map/object");
    };
    character_subrace_name = json_to_parse.at("name").get<std::string>();
    if (character_subrace_name.size() == 0) {
        throw invalid_attribute(ParsingType::SUBRACES, filename, "name", "cannot be \"\".");
    }
    race_name = json_to_parse.at("race").get<std::string>();

    if (!json_to_parse.at("features").is_object()) {
        throw attribute_format_error(ParsingType::SUBRACES, filename, "features", "map/object");
    }
    features.reserve(json_to_parse.size());
    for (const auto& [feature_name, feature_json] : json_to_parse.at("features").items()) {
        features.push_back(std::move(FeatureParser::createFeature(feature_name, feature_json)));
    }
}

bool dnd::CharacterSubraceFileParser::validate() const {
    if (results.find(character_subrace_name) != results.end()) {
        std::cerr << "Warning: Duplicate of subrace \"" << character_subrace_name << "\" found.\n";
        return false;
    }
    if (races.find(race_name) == races.cend()) {
        throw invalid_attribute(
            ParsingType::SUBRACES, filename, "race", "must exist. \"" + race_name + "\" does not exist."
        );
    }
    if (!races.at(race_name)->has_subraces) {
        throw invalid_attribute(
            ParsingType::SUBRACES, filename, "race", "must have subraces. \"" + race_name + "\" does not have subraces."
        );
    }
    return true;
}

void dnd::CharacterSubraceFileParser::saveResult() {
    // TODO: change CharacterSubrace constructor
    auto character_subrace = std::make_shared<CharacterSubrace>(character_subrace_name, race_name);
    character_subrace->features = std::move(features);
    results.emplace(character_subrace_name, std::move(character_subrace));
}

void dnd::CharacterSubraceFileParser::reset() {
    character_subrace_name = "";
    race_name = "";
    features = {};
}
