#ifndef CONTENT_HOLDER_HPP_
#define CONTENT_HOLDER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>

#include <core/controllers/content_library.hpp>
#include <core/controllers/groups.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/feature/choosable_feature.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>

namespace dnd {

/**
 * @brief A class that holds all the content for a certain session or campaign
 */
class ContentHolder {
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
    const StorageContentLibrary<const CharacterClass>& get_character_classes() const;
    const StorageContentLibrary<const CharacterSubclass>& get_character_subclasses() const;
    const StorageContentLibrary<const CharacterRace>& get_character_races() const;
    const StorageContentLibrary<const CharacterSubrace>& get_character_subraces() const;
    const StorageContentLibrary<const Item>& get_items() const;
    const StorageContentLibrary<const Spell>& get_spells() const;
    const ReferencingContentLibrary<const Feature>& get_features() const;
    const StorageContentLibrary<const ChoosableFeature>& get_choosable_features() const;

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
    bool add_choosable_feature(ChoosableFeature&& choosable_feature);
private:
    Groups groups;
    StorageContentLibrary<Character> characters;
    StorageContentLibrary<const CharacterClass> character_classes;
    StorageContentLibrary<const CharacterSubclass> character_subclasses;
    StorageContentLibrary<const CharacterRace> character_races;
    StorageContentLibrary<const CharacterSubrace> character_subraces;
    StorageContentLibrary<const Item> items;
    StorageContentLibrary<const Spell> spells;

    ReferencingContentLibrary<const Feature> features;
    StorageContentLibrary<const ChoosableFeature> choosable_features;
};

} // namespace dnd

#endif // CONTENT_HOLDER_HPP_
