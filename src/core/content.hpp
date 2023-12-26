#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>

#include <core/groups.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_species/character_species.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subspecies/character_subspecies.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/referencing_content_library.hpp>
#include <core/storage_content_library.hpp>
#include <core/utils/types.hpp>

namespace dnd {

using EffectsProviderVariant = VarOfCRef<Feature, ClassFeature, Choosable>;

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
    const StorageContentLibrary<CharacterSpecies>& get_character_species() const;
    const StorageContentLibrary<CharacterSubspecies>& get_character_subspecies() const;
    const StorageContentLibrary<Item>& get_items() const;
    const StorageContentLibrary<Spell>& get_spells() const;
    const ReferencingContentLibrary<Feature>& get_features() const;
    const ReferencingContentLibrary<ClassFeature>& get_class_features() const;
    const StorageContentLibrary<Choosable>& get_choosables() const;

    std::optional<EffectsProviderVariant> get_effects_provider(const std::string& name) const;

    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroups);
    void add_group_member(const std::string& group_name, const std::string& value);
    void add_group_members(const std::string& group_name, std::set<std::string>&& values);

    OptCRef<Character> add_character(Character&& character);
    OptCRef<CharacterClass> add_character_class(CharacterClass&& character_class);
    OptCRef<CharacterSubclass> add_character_subclass(CharacterSubclass&& character_subclass);
    OptCRef<CharacterSpecies> add_character_species(CharacterSpecies&& character_species);
    OptCRef<CharacterSubspecies> add_character_subspecies(CharacterSubspecies&& character_subspecies);
    OptCRef<Item> add_item(Item&& item);
    OptCRef<Spell> add_spell(Spell&& spell);
    OptCRef<Choosable> add_choosable(Choosable&& choosable);
private:
    Groups groups;
    StorageContentLibrary<Character> character_libary;
    StorageContentLibrary<CharacterClass> class_library;
    StorageContentLibrary<CharacterSubclass> subclass_library;
    StorageContentLibrary<CharacterSpecies> species_library;
    StorageContentLibrary<CharacterSubspecies> subspecies_library;
    StorageContentLibrary<Item> item_library;
    StorageContentLibrary<Spell> spell_library;

    ReferencingContentLibrary<Feature> feature_library;
    ReferencingContentLibrary<ClassFeature> class_feature_library;
    StorageContentLibrary<Choosable> choosable_library;
};

} // namespace dnd

#endif // CONTENT_HPP_
