#ifndef CONTENT_HOLDER_HPP_
#define CONTENT_HOLDER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>

#include <core/controllers/content_library.hpp>
#include <core/controllers/groups.hpp>
// #include <core/models/character.hpp>
// #include <core/models/character_class.hpp>
// #include <core/models/character_race.hpp>
// #include <core/models/character_subclass.hpp>
// #include <core/models/character_subrace.hpp>
// #include <core/models/effect_holder/choosable.hpp>
// #include <core/models/effect_holder/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>

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
     * @brief Returns whether all storage libraries are empty
     * @return "true" if nothing is stored inside the content holder, "false" otherwise
     */
    bool empty() const;
    /**
     * @brief Returns a string describing how much content this controller holds
     * @return a string describing the current status of the controller
     */
    std::string status() const;

    // the controller holding all the groups for the campaign
    Groups groups;

    // content libraries for all the types of content

    // StorageContentLibrary<Character> characters;
    // StorageContentLibrary<const CharacterClass> character_classes;
    // StorageContentLibrary<const CharacterSubclass> character_subclasses;
    // StorageContentLibrary<const CharacterRace> character_races;
    // StorageContentLibrary<const CharacterSubrace> character_subraces;
    StorageContentLibrary<const Item> items;
    StorageContentLibrary<const Spell> spells;

    // ReferencingContentLibrary<const Feature> features;
    // std::unordered_map<std::string, ReferencingContentLibrary<const Choosable>> choosables;
};

} // namespace dnd

#endif // CONTENT_HOLDER_HPP_
