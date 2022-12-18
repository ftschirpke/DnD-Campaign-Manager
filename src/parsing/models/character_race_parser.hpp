#ifndef CHARACTER_RACE_PARSER_HPP_
#define CHARACTER_RACE_PARSER_HPP_

#include <memory>

#include <nlohmann/json.hpp>

#include "features/feature_parser.hpp"
#include "../../models/character_race.hpp"

namespace dnd {

class CharacterRaceParser {
public:
    static std::unique_ptr<CharacterRace> createCharacterRace(
        const nlohmann::json& character_race_json);
};

} // namespace dnd

#endif // CHARACTER_RACE_PARSER_HPP_
