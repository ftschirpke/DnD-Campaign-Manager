#ifndef CHARACTER_PARSER_HPP_
#define CHARACTER_PARSER_HPP_

#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "controllers/content_controller.hpp"
#include "models/character.hpp"

namespace dnd {

class CharacterParser {
private:
    static void checkLevel(const std::string& character_name, int level);
    static void checkXP(const std::string& character_name, int xp);
    static void parseFeatureHolders(
        Character& character, const nlohmann::json& character_json, const ContentController& content_controller
    );
    static void parseLevelAndXP(Character& character, const nlohmann::json& character_json);
public:
    static std::shared_ptr<Character> createCharacter(
        const nlohmann::json& character_json, const ContentController& content_controller
    );
};

} // namespace dnd

#endif // CHARACTER_PARSER_HPP_
