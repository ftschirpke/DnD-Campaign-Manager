#ifndef CHARACTER_PARSER_HPP_
#define CHARACTER_PARSER_HPP_

#include <memory>

#include <nlohmann/json.hpp>

#include "models/character.hpp"

namespace dnd {

class CharacterParser {
public:
    static std::unique_ptr<Character> createCharacter(const nlohmann::json& character_json);
};

} // namespace dnd

#endif // CHARACTER_PARSER_HPP_
