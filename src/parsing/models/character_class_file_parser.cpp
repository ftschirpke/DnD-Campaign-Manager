#include "character_class_file_parser.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterClassFileParser::parse() {
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::CLASSES, filename, "map/object");
    }
    character_class_name = json_to_parse.at("name").get<std::string>();
    if (character_class_name.size() == 0) {
        throw invalid_attribute(ParsingType::CLASSES, filename, "name", "cannot be \"\".");
    }
    character_class_hit_dice = json_to_parse.at("hit_dice").get<std::string>();
    // TODO: change int to short
    asi_levels = json_to_parse.at("asi_levels").get<std::vector<int>>();

    features = parseFeatures();
}

bool dnd::CharacterClassFileParser::validate() const {
    if (results.find(character_class_name) != results.end()) {
        std::cerr << "Warning: Duplicate of class \"" << character_class_name << "\" found.\n";
        return false;
    }
    return true;
}

void dnd::CharacterClassFileParser::saveResult() {
    // TODO: change CharacterClass constructor
    auto character_class = std::make_shared<CharacterClass>(character_class_name, character_class_hit_dice, asi_levels);
    character_class->features = std::move(features);
    results.emplace(character_class_name, std::move(character_class));
}

void dnd::CharacterClassFileParser::reset() {
    character_class_name = "";
    character_class_hit_dice = "";
    asi_levels = {};
    features = {};
}