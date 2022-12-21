#ifndef CHARACTER_CLASS_PARSER_HPP_
#define CHARACTER_CLASS_PARSER_HPP_

#include <memory>

#include <nlohmann/json.hpp>

#include "models/character_class.hpp"
#include "parsing/models/features/feature_parser.hpp"

namespace dnd {

class CharacterClassParser {
public:
    static std::shared_ptr<const CharacterClass> createCharacterClass(const nlohmann::json& character_class_json);
    static std::shared_ptr<const CharacterSubclass> createCharacterSubclass(
        const nlohmann::json& character_subclass_json
    );
};

} // namespace dnd

#endif // CHARACTER_CLASS_PARSER_HPP_
