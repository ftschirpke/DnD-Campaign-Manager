#ifndef CHARACTER_CLASS_PARSER_HPP_
#define CHARACTER_CLASS_PARSER_HPP_

#include <memory>

#include <nlohmann/json.hpp>

#include "features/feature_parser.hpp"
#include "../../models/character_class.hpp"

namespace dnd {

class CharacterClassParser {
public:
    static std::unique_ptr<CharacterClass> createCharacterClass(
        const nlohmann::json& character_class_json);
    static std::unique_ptr<CharacterSubclass> createCharacterSubclass(
        const nlohmann::json& character_subclass_json);
};

} // namespace dnd

#endif // CHARACTER_CLASS_PARSER_HPP_
