#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/spell.hpp"

namespace dnd {

class Content {
public:
    std::unordered_map<std::string, Character> characters;
    std::unordered_map<std::string, const CharacterClass> character_classes;
    std::unordered_map<std::string, const CharacterSubclass> character_subclasses;
    std::unordered_map<std::string, const CharacterRace> character_races;
    std::unordered_map<std::string, const CharacterSubrace> character_subraces;
    std::unordered_map<std::string, const Spell> spells;
    void printStatus() const;
};

} // namespace dnd

#endif // CONTENT_HPP_
