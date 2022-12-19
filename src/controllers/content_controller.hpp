#ifndef CONTENT_CONTROLLER_HPP_
#define CONTENT_CONTROLLER_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "models/spell.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"

namespace dnd {
    
class ContentController {
public:
    std::unordered_map<std::string, std::unique_ptr<Spell>> spells;
    std::unordered_map<std::string, std::unique_ptr<CharacterClass>> character_classes;
    std::unordered_map<std::string, std::unique_ptr<CharacterSubclass>> character_subclasses;
    std::unordered_map<std::string, std::unique_ptr<CharacterRace>> character_races;
    std::unordered_map<std::string, std::unique_ptr<CharacterSubrace>> character_subraces;
    void printStatus() const;
};

} // namespace dnd

#endif // CONTENT_CONTROLLER_HPP_
