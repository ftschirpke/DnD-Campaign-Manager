#ifndef CONTENT_HOLDER_HPP_
#define CONTENT_HOLDER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>

#include "controllers/content_library.hpp"
#include "controllers/groups.hpp"
#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_subclass.hpp"
#include "models/character_subrace.hpp"
#include "models/effect_holder/choosable.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/item.hpp"
#include "models/spell.hpp"

namespace dnd {

/**
 * @brief A class that holds all the content for a certain session or campaign
 */
class ContentHolder {
public:
    /**
     * @brief Should be called when you finished parsing to allow automatic collection of features etc. out of the other
     * parsed content types
     */
    void finished_parsing();
    /**
     * @brief Returns a string describing how much content this controller holds
     * @return a string describing the current status of the controller
     */
    std::string printStatus() const;

    // the controller holding all the groups for the campaign
    Groups groups;

    // content libraries for all the types of content

    ContentLibrary<Character> characters;
    ContentLibrary<const CharacterClass> character_classes;
    ContentLibrary<const CharacterSubclass> character_subclasses;
    ContentLibrary<const CharacterRace> character_races;
    ContentLibrary<const CharacterSubrace> character_subraces;
    ContentLibrary<const Item> items;
    ContentLibrary<const Spell> spells;

    ContentLibrary<const Feature*> features;
    std::unordered_map<std::string, ContentLibrary<const Choosable*>> choosables;
};

} // namespace dnd

#endif // CONTENT_HOLDER_HPP_
