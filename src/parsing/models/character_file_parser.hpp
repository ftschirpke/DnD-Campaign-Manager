#ifndef CHARACTER_FILE_PARSER_HPP_
#define CHARACTER_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_subclass.hpp"
#include "models/character_subrace.hpp"
#include "models/effect_holder/character_decision.hpp"
#include "models/spell.hpp"
#include "parsing/models/effect_holder/effect_holder_parser.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

class CharacterFileParser : public FeatureHolderFileParser {
public:
    CharacterFileParser(
        std::unordered_map<std::string, Character>& results, const Groups& groups,
        const std::unordered_map<std::string, const CharacterClass>& character_classes,
        const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses,
        const std::unordered_map<std::string, const CharacterRace>& character_races,
        const std::unordered_map<std::string, const CharacterSubrace>& character_subraces,
        const std::unordered_map<std::string, const Spell>& spells
    ) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
protected:
    void parseCharacterDecisions(const std::string& feature_name, const nlohmann::json& feature_decisions_json);
private:
    static const ParsingType type;
    std::unordered_map<std::string, Character>& results;
    const std::unordered_map<std::string, const CharacterClass>& character_classes;
    const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses;
    const std::unordered_map<std::string, const CharacterRace>& character_races;
    const std::unordered_map<std::string, const CharacterSubrace>& character_subraces;
    const std::unordered_map<std::string, const Spell>& spells;
    std::string character_name;
    std::array<unsigned int, 6> base_ability_scores;
    std::vector<unsigned int> hit_dice_rolls;
    const CharacterClass* class_ptr;
    const CharacterSubclass* subclass_ptr;
    const CharacterRace* race_ptr;
    const CharacterSubrace* subrace_ptr;
    std::vector<CharacterDecision> decisions;
    unsigned int level, xp;
    EffectHolderParser effect_holder_parser;
    virtual void configureSubparsers() override;
    void parseClassAndRace();
    void parseLevelAndXP();
};

inline CharacterFileParser::CharacterFileParser(
    std::unordered_map<std::string, Character>& results, const Groups& groups,
    const std::unordered_map<std::string, const CharacterClass>& character_classes,
    const std::unordered_map<std::string, const CharacterSubclass>& character_subclasses,
    const std::unordered_map<std::string, const CharacterRace>& character_races,
    const std::unordered_map<std::string, const CharacterSubrace>& character_subraces,
    const std::unordered_map<std::string, const Spell>& spells
) noexcept
    : FeatureHolderFileParser(groups), results(results), character_classes(character_classes),
      character_subclasses(character_subclasses), character_races(character_races),
      character_subraces(character_subraces), spells(spells), class_ptr(nullptr), subclass_ptr(nullptr),
      race_ptr(nullptr), subrace_ptr(nullptr), effect_holder_parser(groups) {}

inline const ParsingType CharacterFileParser::type = ParsingType::CHARACTER;

} // namespace dnd

#endif // CHARACTER_FILE_PARSER_HPP_
