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
     * @brief Returns whether the content holder is empty
     * @return true if the content holder is empty, false otherwise
     */
    bool empty() const;
    /**
     * @brief Returns a string describing how much content this controller holds
     * @return a string describing the current status of the controller
     */
    std::string printStatus() const;

    // the controller holding all the groups for the campaign
    Groups groups;

    // content libraries for all the types of content

    StoringContentLibrary<Character> characters;
    StoringContentLibrary<const CharacterClass> character_classes;
    StoringContentLibrary<const CharacterSubclass> character_subclasses;
    StoringContentLibrary<const CharacterRace> character_races;
    StoringContentLibrary<const CharacterSubrace> character_subraces;
    StoringContentLibrary<const Item> items;
    StoringContentLibrary<const Spell> spells;

    ReferencingContentLibrary<const Feature> features;
    std::unordered_map<std::string, ReferencingContentLibrary<const Choosable>> choosables;
};

inline bool ContentHolder::empty() const {
    return characters.empty() && character_classes.empty() && character_subclasses.empty() && character_races.empty()
           && character_subraces.empty() && items.empty() && spells.empty();
}

} // namespace dnd

#endif // CONTENT_HOLDER_HPP_
