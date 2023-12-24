#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include <dnd_config.hpp>

#include <functional>
#include <optional>
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
    const StorageContentLibrary<Choosable>& get_choosables() const;

    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroups);
    void add_group_member(const std::string& group_name, const std::string& value);
    void add_group_members(const std::string& group_name, std::set<std::string>&& values);

    std::optional<std::reference_wrapper<const Character>> add_character(Character&& character);
    std::optional<std::reference_wrapper<const CharacterClass>> add_character_class(CharacterClass&& character_class);
    std::optional<std::reference_wrapper<const CharacterSubclass>> add_character_subclass(
        CharacterSubclass&& character_subclass
    );
    std::optional<std::reference_wrapper<const CharacterRace>> add_character_race(CharacterRace&& character_race);
    std::optional<std::reference_wrapper<const CharacterSubrace>> add_character_subrace(
        CharacterSubrace&& character_subrace
    );
    std::optional<std::reference_wrapper<const Item>> add_item(Item&& item);
    std::optional<std::reference_wrapper<const Spell>> add_spell(Spell&& spell);
    std::optional<std::reference_wrapper<const Choosable>> add_choosable(Choosable&& choosable);
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
