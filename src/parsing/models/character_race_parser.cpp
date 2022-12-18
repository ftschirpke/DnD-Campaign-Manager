#include "character_race_parser.hpp"

#include <memory>

#include <nlohmann/json.hpp>

#include "feature_holder_parser.hpp"
#include "features/feature_parser.hpp"
#include "../../models/character_race.hpp"

std::unique_ptr<dnd::CharacterRace> dnd::CharacterRaceParser::createCharacterRace(
    const nlohmann::json& character_race_json) {
    if (!character_race_json.is_object()) {
        throw std::invalid_argument("Race is not formatted as an object/map.");
    }
    bool has_subraces = false;
    if (character_race_json.contains("has_subraces")) {
        has_subraces = character_race_json.at("has_subraces");
    }
    CharacterRace character_race(character_race_json.at("name"), has_subraces);
    FeatureHolderParser::parseAndAddFeatures(
        character_race_json.at("features"), character_race
    );
    return std::make_unique<CharacterRace>(std::move(character_race));
}
