#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>

#include <core/groups.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/feature/choosable.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/referencing_content_library.hpp>
#include <core/storage_content_library.hpp>

namespace dnd {

/**
 * @brief A class that holds all the content for a certain session or campaign
 */
class Content {
public:
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

    const Groups& get_groups() const;
    const StorageContentLibrary<Character>& get_characters() const;
    const StorageContentLibrary<CharacterClass>& get_character_classes() const;
    const StorageContentLibrary<CharacterSubclass>& get_character_subclasses() const;
    const StorageContentLibrary<CharacterRace>& get_character_races() const;
    const StorageContentLibrary<CharacterSubrace>& get_character_subraces() const;
    const StorageContentLibrary<Item>& get_items() const;
    const StorageContentLibrary<Spell>& get_spells() const;
    const ReferencingContentLibrary<Feature>& get_features() const;
    const StorageContentLibrary<Choosable>& get_choosables() const;

    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroups);
    void add_group_member(const std::string& group_name, const std::string& value);
    void add_group_members(const std::string& group_name, std::set<std::string>&& values);

    bool add_character(Character&& character);
    bool add_character_class(CharacterClass&& character_class);
    bool add_character_subclass(CharacterSubclass&& character_subclass);
    bool add_character_race(CharacterRace&& character_race);
    bool add_character_subrace(CharacterSubrace&& character_subrace);
    bool add_item(Item&& item);
    bool add_spell(Spell&& spell);
    bool add_choosable(Choosable&& choosable);
private:
    Groups groups;
    StorageContentLibrary<Character> characters;
    StorageContentLibrary<CharacterClass> character_classes;
    StorageContentLibrary<CharacterSubclass> character_subclasses;
    StorageContentLibrary<CharacterRace> character_races;
    StorageContentLibrary<CharacterSubrace> character_subraces;
    StorageContentLibrary<Item> items;
    StorageContentLibrary<Spell> spells;

    ReferencingContentLibrary<Feature> features;
    StorageContentLibrary<Choosable> choosables;
};

} // namespace dnd

#endif // CONTENT_HPP_
