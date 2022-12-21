#ifndef CONTENT_CONTROLLER_HPP_
#define CONTENT_CONTROLLER_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/spell.hpp"

namespace dnd {

class ContentController {
public:
    std::unordered_map<std::string, std::shared_ptr<const Spell>> spells;
    std::unordered_map<std::string, std::shared_ptr<const CharacterClass>> character_classes;
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubclass>> character_subclasses;
    std::unordered_map<std::string, std::shared_ptr<const CharacterRace>> character_races;
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubrace>> character_subraces;
    void printStatus() const;
};

} // namespace dnd

#endif // CONTENT_CONTROLLER_HPP_
