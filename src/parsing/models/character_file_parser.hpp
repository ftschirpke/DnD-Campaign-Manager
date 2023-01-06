#ifndef CHARACTER_FILE_PARSER_HPP_
#define CHARACTER_FILE_PARSER_HPP_

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "controllers/content.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/creature_state.hpp"
#include "models/spell.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

class CharacterFileParser : public FeatureHolderFileParser {
private:
    std::unordered_map<std::string, Character>& results;
    const std::unordered_map<std::string, const CharacterClass>& character_classes;
    const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses;
    const std::unordered_map<std::string, const CharacterRace>& character_races;
    const std::unordered_map<std::string, const CharacterSubrace>& character_subraces;
    const std::unordered_map<std::string, const Spell>& spells;
    std::string character_name;
    std::array<int, 6> base_ability_scores;
    std::vector<int> hit_dice_rolls;
    const CharacterClass* class_ptr;
    const CharacterSubclass* subclass_ptr;
    const CharacterRace* race_ptr;
    const CharacterSubrace* subrace_ptr;
    int level, xp;
    void parseClassAndRace();
    void parseLevelAndXP();
public:
    CharacterFileParser(
        std::unordered_map<std::string, Character>& results,
        const std::unordered_map<std::string, const CharacterClass>& character_classes,
        const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses,
        const std::unordered_map<std::string, const CharacterRace>& character_races,
        const std::unordered_map<std::string, const CharacterSubrace>& character_subraces,
        const std::unordered_map<std::string, const Spell>& spells
    );
    void parse() override;
    bool validate() const override;
    void saveResult() override;
};

inline CharacterFileParser::CharacterFileParser(
    std::unordered_map<std::string, Character>& results,
    const std::unordered_map<std::string, const CharacterClass>& character_classes,
    const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses,
    const std::unordered_map<std::string, const CharacterRace>& character_races,
    const std::unordered_map<std::string, const CharacterSubrace>& character_subraces,
    const std::unordered_map<std::string, const Spell>& spells
)
    : results(results), character_classes(character_classes), character_subclasses(character_subclasses),
      character_races(character_races), character_subraces(character_subraces), spells(spells) {}

} // namespace dnd

#endif // CHARACTER_FILE_PARSER_HPP_
