#ifndef CHARACTER_CLASS_PARSER_HPP_
#define CHARACTER_CLASS_PARSER_HPP_

#include <memory>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "models/character_class.hpp"
#include "models/features/feature.hpp"
#include "parsing/models/content_type_parser.hpp"
// #include "parsing/models/features/feature_parser.hpp"

namespace dnd {

class CharacterClassParser : public ContentTypeParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results_map;
    std::string character_class_name, character_class_hit_dice;
    std::vector<int> asi_levels;
    std::vector<std::shared_ptr<const Feature>> features;
public:
    CharacterClassParser(std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results_map);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
    void reset() override;
    // static std::shared_ptr<const CharacterClass> createCharacterClass(const nlohmann::json& character_class_json);
    // static std::shared_ptr<const CharacterSubclass> createCharacterSubclass(
    //     const nlohmann::json& character_subclass_json
    // );
};

CharacterClassParser::CharacterClassParser(
    std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& results_map
)
    : results_map(results_map) {}

} // namespace dnd

#endif // CHARACTER_CLASS_PARSER_HPP_
