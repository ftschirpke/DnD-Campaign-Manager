#ifndef CONTENT_HPP_
#define CONTENT_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>

#include <core/groups.hpp>
#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
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
    const StorageContentLibrary<Class>& get_classes() const;
    const StorageContentLibrary<Subclass>& get_subclasses() const;
    const StorageContentLibrary<Species>& get_species() const;
    const StorageContentLibrary<Subspecies>& get_subspecies() const;
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
    OptCRef<Class> add_class(Class&& classv);
    OptCRef<Subclass> add_subclass(Subclass&& subclass);
    OptCRef<Species> add_species(Species&& species);
    OptCRef<Subspecies> add_subspecies(Subspecies&& subspecies);
    OptCRef<Item> add_item(Item&& item);
    OptCRef<Spell> add_spell(Spell&& spell);
    OptCRef<Choosable> add_choosable(Choosable&& choosable);
private:
    Groups groups;
    StorageContentLibrary<Character> character_libary;
    StorageContentLibrary<Class> class_library;
    StorageContentLibrary<Subclass> subclass_library;
    StorageContentLibrary<Species> species_library;
    StorageContentLibrary<Subspecies> subspecies_library;
    StorageContentLibrary<Item> item_library;
    StorageContentLibrary<Spell> spell_library;

    ReferencingContentLibrary<Feature> feature_library;
    ReferencingContentLibrary<ClassFeature> class_feature_library;
    StorageContentLibrary<Choosable> choosable_library;
};

} // namespace dnd

#endif // CONTENT_HPP_
