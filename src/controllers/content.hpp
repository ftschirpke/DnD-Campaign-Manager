#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>

#include "controllers/groups.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_subclass.hpp"
#include "models/character_subrace.hpp"
#include "models/spell.hpp"

namespace dnd {

/**
 * @brief A class that holds all the content for a certain session or campaign
 */
class Content {
public:
    /**
     * @brief Prints how much content this controller holds
     */
    void printStatus() const;

    // the controller holding all the groups for the campaign
    Groups groups;
    // the characters mapped to their names
    std::unordered_map<std::string, Character> characters;
    // the character classes mapped to their names
    std::unordered_map<std::string, const CharacterClass> character_classes;
    // the character subclasses mapped to their names
    std::unordered_map<std::string, const CharacterSubclass> character_subclasses;
    // the character races mapped to their names
    std::unordered_map<std::string, const CharacterRace> character_races;
    // the character subraces mapped to their names
    std::unordered_map<std::string, const CharacterSubrace> character_subraces;
    // the spells to their names
    std::unordered_map<std::string, const Spell> spells;
};

} // namespace dnd

#endif // CONTENT_HPP_
