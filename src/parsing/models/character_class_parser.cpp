#include "character_class_parser.hpp"

#include <memory>

#include <nlohmann/json.hpp>

#include "models/character_class.hpp"
#include "parsing/models/feature_holder_parser.hpp"
#include "parsing/models/features/feature_parser.hpp"

std::shared_ptr<const dnd::CharacterClass> dnd::CharacterClassParser::createCharacterClass(
    const nlohmann::json& character_class_json
) {
    const std::string character_class_name = character_class_json.at("name").get<std::string>();
    const std::string character_class_hit_dice = character_class_json.at("hit_dice").get<std::string>();
    CharacterClass character_class(character_class_name, character_class_hit_dice);
    FeatureHolderParser::parseAndAddFeatures(
        character_class_json.at("features"), character_class
    );
    return std::make_shared<const CharacterClass>(std::move(character_class));
}

std::shared_ptr<const dnd::CharacterSubclass> dnd::CharacterClassParser::createCharacterSubclass(
    const nlohmann::json& character_subclass_json
) {
    const std::string character_subclass_name = character_subclass_json.at("name").get<std::string>();
    const std::string character_subclass_class_name = character_subclass_json.at("class").get<std::string>();
    CharacterSubclass character_subclass(character_subclass_name, character_subclass_class_name);
    FeatureHolderParser::parseAndAddFeatures(character_subclass_json.at("features"), character_subclass);
    return std::make_shared<const CharacterSubclass>(std::move(character_subclass));
}
