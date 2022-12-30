#include "character_class_parser.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/character_class.hpp"
#include "parsing/models/content_type_parser.hpp"
#include "parsing/models/feature_holder_parser.hpp"
#include "parsing/models/features/feature_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterClassParser::parse() {
    if (!json_to_parse.is_object()) {
        throw json_format_error(filename, "map/object", ParsingType::CLASSES);
    }
    character_class_name = json_to_parse.at("name").get<std::string>();
    if (character_class_name.size() == 0) {
        throw invalid_attribute(filename, "name", "cannot be \"\".");
    }
    character_class_hit_dice = json_to_parse.at("hit_dice").get<std::string>();
    // TODO: change int to short
    asi_levels = json_to_parse.at("asi_levels").get<std::vector<int>>();
    features = FeatureHolderParser::parseAndAddFeatures(json_to_parse.at("features"));
}

bool dnd::CharacterClassParser::validate() const {
    if (results_map.find(character_class_name) != results_map.end()) {
        std::cerr << "Warning: Duplicate of class \"" << character_class_name << "\" found.\n";
        return false;
    }
    return true;
}

void dnd::CharacterClassParser::saveResult() {
    // TODO: change CharacterClass constructor
    auto character_class = std::make_shared<CharacterClass>(character_class_name, character_class_hit_dice, asi_levels);
    character_class->features = std::move(features);
    results_map.emplace(character_class_name, std::move(character_class));
}

void dnd::CharacterClassParser::reset() {
    character_class_name = nullptr;
    character_class_hit_dice = nullptr;
    asi_levels = {};
    features = {};
}

// std::shared_ptr<const dnd::CharacterClass> dnd::CharacterClassParser::createCharacterClass(
//     const nlohmann::json& character_class_json
// ) {
//     const std::string character_class_name = character_class_json.at("name").get<std::string>();
//     const std::string character_class_hit_dice = character_class_json.at("hit_dice").get<std::string>();
//     const std::vector<int> asi_levels = character_class_json.at("asi_levels").get<std::vector<int>>();
//     CharacterClass character_class(character_class_name, character_class_hit_dice, asi_levels);
//     FeatureHolderParser::parseAndAddFeatures(character_class_json.at("features"), character_class);
//     return std::make_shared<const CharacterClass>(std::move(character_class));
// }

// std::shared_ptr<const dnd::CharacterSubclass> dnd::CharacterClassParser::createCharacterSubclass(
//     const nlohmann::json& character_subclass_json
// ) {
//     const std::string character_subclass_name = character_subclass_json.at("name").get<std::string>();
//     const std::string character_subclass_class_name = character_subclass_json.at("class").get<std::string>();
//     CharacterSubclass character_subclass(character_subclass_name, character_subclass_class_name);
//     FeatureHolderParser::parseAndAddFeatures(character_subclass_json.at("features"), character_subclass);
//     return std::make_shared<const CharacterSubclass>(std::move(character_subclass));
// }
