#include "character_class_parser.hpp"

#include <memory>

#include <nlohmann/json.hpp>

#include "models/character_class.hpp"
#include "parsing/models/feature_holder_parser.hpp"
#include "parsing/models/features/feature_parser.hpp"

std::unique_ptr<dnd::CharacterClass> dnd::CharacterClassParser::createCharacterClass(
    const nlohmann::json& character_class_json) {
    if (!character_class_json.is_object()) {
        throw std::invalid_argument("Class is not formatted as an object/map.");
    }
    CharacterClass character_class(character_class_json.at("name"), character_class_json.at("hit_dice"));
    FeatureHolderParser::parseAndAddFeatures(
        character_class_json.at("features"), character_class
    );
    return std::make_unique<CharacterClass>(std::move(character_class));
}

std::unique_ptr<dnd::CharacterSubclass> dnd::CharacterClassParser::createCharacterSubclass(
    const nlohmann::json& character_subclass_json) {
    if (!character_subclass_json.is_object()) {
        throw std::invalid_argument("Subclass is not formatted as an object/map.");
    }
    CharacterSubclass character_subclass(
        character_subclass_json.at("name"), character_subclass_json.at("class")
    );
    FeatureHolderParser::parseAndAddFeatures(
        character_subclass_json.at("features"), character_subclass
    );
    return std::make_unique<CharacterSubclass>(std::move(character_subclass));
}
