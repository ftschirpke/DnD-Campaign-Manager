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
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/referencing_content_library.hpp>
#include <core/storage_content_library.hpp>
#include <core/utils/types.hpp>

namespace dnd {

enum class EffectsProviderType {
    Feature,
    ClassFeature,
    Choosable,
};

/**
 * @brief A class that holds all the content for a certain session or campaign
 */
class Content {
public:
    bool empty() const;
    /**
     * @brief Returns a string describing how many items this controller holds
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
    const ReferencingContentLibrary<ClassFeature>& get_class_features() const;
    const StorageContentLibrary<Choosable>& get_choosables() const;

    std::optional<EffectsProviderType> contains_effects_provider(const std::string& name) const;
    OptCRef<EffectsProvider> get_effects_provider(const std::string& name) const;

    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroups);
    void add_group_member(const std::string& group_name, const std::string& value);
    void add_group_members(const std::string& group_name, std::set<std::string>&& values);

    OptCRef<Character> add_character(Character&& character);
    OptCRef<CharacterClass> add_character_class(CharacterClass&& character_class);
    OptCRef<CharacterSubclass> add_character_subclass(CharacterSubclass&& character_subclass);
    OptCRef<CharacterRace> add_character_race(CharacterRace&& character_race);
    OptCRef<CharacterSubrace> add_character_subrace(CharacterSubrace&& character_subrace);
    OptCRef<Item> add_item(Item&& item);
    OptCRef<Spell> add_spell(Spell&& spell);
    OptCRef<Choosable> add_choosable(Choosable&& choosable);
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
    ReferencingContentLibrary<ClassFeature> class_features;
    StorageContentLibrary<Choosable> choosables;
};

} // namespace dnd

#endif // CONTENT_HPP_
