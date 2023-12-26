#include <dnd_config.hpp>

#include "content.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

#include <fmt/format.h>

#include <core/referencing_content_library.hpp>
#include <core/storage_content_library.hpp>
#include <core/utils/types.hpp>

bool dnd::Content::empty() const {
    return character_libary.empty() && class_library.empty() && subclass_library.empty() && species_library.empty()
           && subspecies_library.empty() && item_library.empty() && spell_library.empty();
}

std::string dnd::Content::status() const {
    return fmt::format(
        "{}\n=== Items ===\nitems parsed: {}\n=== Spells ===\nspells parsed: {}\n=== Character Species ===\ncharacter "
        "species parsed: {}\ncharacter subspecies parsed: {}\n=== Character Classes ===\ncharacter classes parsed: "
        "{}\ncharacter subclasses parsed: {}\n=== Characters ===\ncharacters parsed: {}",
        groups.status(), item_library.size(), spell_library.size(), species_library.size(), subspecies_library.size(),
        class_library.size(), subclass_library.size(), character_libary.size()
    );
}

const dnd::Groups& dnd::Content::get_groups() const { return groups; }

const dnd::StorageContentLibrary<dnd::Character>& dnd::Content::get_characters() const { return character_libary; }

const dnd::StorageContentLibrary<dnd::CharacterClass>& dnd::Content::get_character_classes() const {
    return class_library;
}

const dnd::StorageContentLibrary<dnd::CharacterSubclass>& dnd::Content::get_character_subclasses() const {
    return subclass_library;
}

const dnd::StorageContentLibrary<dnd::CharacterSpecies>& dnd::Content::get_character_species() const {
    return species_library;
}

const dnd::StorageContentLibrary<dnd::CharacterSubspecies>& dnd::Content::get_character_subspecies() const {
    return subspecies_library;
}

const dnd::StorageContentLibrary<dnd::Item>& dnd::Content::get_items() const { return item_library; }

const dnd::StorageContentLibrary<dnd::Spell>& dnd::Content::get_spells() const { return spell_library; }

const dnd::ReferencingContentLibrary<dnd::Feature>& dnd::Content::get_features() const { return feature_library; }

const dnd::ReferencingContentLibrary<dnd::ClassFeature>& dnd::Content::get_class_features() const {
    return class_feature_library;
}

const dnd::StorageContentLibrary<dnd::Choosable>& dnd::Content::get_choosables() const { return choosable_library; }

std::optional<dnd::EffectsProviderVariant> dnd::Content::get_effects_provider(const std::string& name) const {
    OptCRef<Feature> feature = feature_library.get(name);
    if (feature.has_value()) {
        return feature.value();
    }
    OptCRef<ClassFeature> class_feature = class_feature_library.get(name);
    if (class_feature.has_value()) {
        return class_feature.value();
    }
    OptCRef<Choosable> choosable = choosable_library.get(name);
    if (choosable.has_value()) {
        return choosable.value();
    }
    return std::nullopt;
}

void dnd::Content::set_subgroup(const std::string& group_name, const std::string& subgroup_name) {
    groups.set_subgroup(group_name, subgroup_name);
}

void dnd::Content::set_subgroups(const std::string& group_name, std::set<std::string>&& subgroup_names) {
    groups.set_subgroups(group_name, std::move(subgroup_names));
}

void dnd::Content::add_group_member(const std::string& group_name, const std::string& value) {
    groups.add(group_name, value);
}

void dnd::Content::add_group_members(const std::string& group_name, std::set<std::string>&& values) {
    groups.add(group_name, std::move(values));
}

dnd::OptCRef<dnd::Character> dnd::Content::add_character(dnd::Character&& character) {
    const std::string name = character.get_name();
    OptCRef<Character> inserted_character = character_libary.add(std::move(character));
    if (inserted_character.has_value()) {
        for (const Feature& feature : inserted_character.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_character;
}

dnd::OptCRef<dnd::CharacterClass> dnd::Content::add_character_class(dnd::CharacterClass&& character_class) {
    const std::string name = character_class.get_name();
    OptCRef<CharacterClass> inserted_class = class_library.add(std::move(character_class));
    if (inserted_class.has_value()) {
        for (const ClassFeature& feature : inserted_class.value().get().get_features()) {
            class_feature_library.add(feature);
        }
    }
    return inserted_class;
}

dnd::OptCRef<dnd::CharacterSubclass> dnd::Content::add_character_subclass(dnd::CharacterSubclass&& character_subclass) {
    const std::string name = character_subclass.get_name();
    OptCRef<CharacterSubclass> inserted_subclass = subclass_library.add(std::move(character_subclass));
    if (subclass_library.add(std::move(character_subclass))) {
        for (const ClassFeature& class_feature : inserted_subclass.value().get().get_features()) {
            class_feature_library.add(class_feature);
        }
    }
    return inserted_subclass;
}

dnd::OptCRef<dnd::CharacterSpecies> dnd::Content::add_character_species(dnd::CharacterSpecies&& character_species) {
    const std::string name = character_species.get_name();
    OptCRef<CharacterSpecies> inserted_species = species_library.add(std::move(character_species));
    if (inserted_species.has_value()) {
        for (const Feature& feature : inserted_species.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_species;
}

dnd::OptCRef<dnd::CharacterSubspecies> dnd::Content::add_character_subspecies(
    dnd::CharacterSubspecies&& character_subspecies
) {
    const std::string name = character_subspecies.get_name();
    OptCRef<CharacterSubspecies> inserted_subspecies = subspecies_library.add(std::move(character_subspecies));
    if (inserted_subspecies.has_value()) {
        for (const Feature& feature : inserted_subspecies.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_subspecies;
}

dnd::OptCRef<dnd::Item> dnd::Content::add_item(dnd::Item&& item) { return item_library.add(std::move(item)); }

dnd::OptCRef<dnd::Spell> dnd::Content::add_spell(dnd::Spell&& spell) { return spell_library.add(std::move(spell)); }

dnd::OptCRef<dnd::Choosable> dnd::Content::add_choosable(dnd::Choosable&& choosable) {
    const std::string name = choosable.get_name();
    const std::string type_name = choosable.get_type();
    OptCRef<Choosable> inserted_choosable = choosable_library.add(std::move(choosable));
    if (inserted_choosable.has_value()) {
        groups.add(type_name, name);
    }
    return inserted_choosable;
}
