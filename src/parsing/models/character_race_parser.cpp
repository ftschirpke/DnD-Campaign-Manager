#include "character_race_parser.hpp"

#include <memory>

#include <nlohmann/json.hpp>

#include "models/character_race.hpp"
#include "parsing/models/feature_holder_parser.hpp"
#include "parsing/models/features/feature_parser.hpp"

std::shared_ptr<const dnd::CharacterRace> dnd::CharacterRaceParser::createCharacterRace(
    const nlohmann::json& character_race_json
) {
    bool has_subraces = false;
    if (character_race_json.contains("has_subraces")) {
        has_subraces = character_race_json.at("has_subraces");
    }
    CharacterRace character_race(character_race_json.at("name"), has_subraces);
    FeatureHolderParser::parseAndAddFeatures(character_race_json.at("features"), character_race);
    return std::make_shared<const CharacterRace>(std::move(character_race));
}

std::shared_ptr<const dnd::CharacterSubrace> dnd::CharacterRaceParser::createCharacterSubrace(
    const nlohmann::json& character_subrace_json
) {
    CharacterSubrace character_subrace(character_subrace_json.at("name"), character_subrace_json.at("race"));
    FeatureHolderParser::parseAndAddFeatures(character_subrace_json.at("features"), character_subrace);
    return std::make_shared<const CharacterSubrace>(std::move(character_subrace));
}
