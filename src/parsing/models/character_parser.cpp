#include "character_parser.hpp"

#include <memory>

#include <nlohmann/json.hpp>

#include "models/character.hpp"

std::unique_ptr<dnd::Character> dnd::CharacterParser::createCharacter(const nlohmann::json& character_json) {
    Character character("test character", {});
    return std::make_unique<Character>(std::move(character));
}
