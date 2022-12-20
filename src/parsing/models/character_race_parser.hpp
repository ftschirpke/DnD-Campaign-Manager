#ifndef CHARACTER_RACE_PARSER_HPP_
#define CHARACTER_RACE_PARSER_HPP_

#include <memory>

#include <nlohmann/json.hpp>

#include "models/character_race.hpp"
#include "parsing/models/features/feature_parser.hpp"

namespace dnd {

class CharacterRaceParser {
public:
    static std::unique_ptr<CharacterRace> createCharacterRace(const nlohmann::json& character_race_json);
    static std::unique_ptr<CharacterSubrace> createCharacterSubrace(const nlohmann::json& character_subrace_json);
};

} // namespace dnd

#endif // CHARACTER_RACE_PARSER_HPP_
