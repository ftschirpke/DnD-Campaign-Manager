#include <dnd_config.hpp>

#include "content.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

#include <fmt/format.h>

#include <core/referencing_content_library.hpp>
#include <core/storage_content_library.hpp>
#include <core/utils/types.hpp>

namespace dnd {

bool Content::empty() const {
    return character_libary.empty() && class_library.empty() && subclass_library.empty() && species_library.empty()
           && subspecies_library.empty() && item_library.empty() && spell_library.empty();
}

std::string Content::status() const {
    return fmt::format(
        "{}\n=== Items ===\nitems parsed: {}\n=== Spells ===\nspells parsed: {}\n=== Character Species ===\ncharacter "
        "species parsed: {}\ncharacter subspecies parsed: {}\n=== Character Classes ===\ncharacter classes parsed: "
        "{}\ncharacter subclasses parsed: {}\n=== Characters ===\ncharacters parsed: {}",
        groups.status(), item_library.size(), spell_library.size(), species_library.size(), subspecies_library.size(),
        class_library.size(), subclass_library.size(), character_libary.size()
    );
}

const Groups& Content::get_groups() const { return groups; }

const StorageContentLibrary<Character>& Content::get_characters() const { return character_libary; }

const StorageContentLibrary<Class>& Content::get_classes() const { return class_library; }

const StorageContentLibrary<Subclass>& Content::get_subclasses() const { return subclass_library; }

const StorageContentLibrary<Species>& Content::get_species() const { return species_library; }

const StorageContentLibrary<Subspecies>& Content::get_subspecies() const { return subspecies_library; }

const StorageContentLibrary<Item>& Content::get_items() const { return item_library; }

const StorageContentLibrary<Spell>& Content::get_spells() const { return spell_library; }

const ReferencingContentLibrary<Feature>& Content::get_features() const { return feature_library; }

const ReferencingContentLibrary<ClassFeature>& Content::get_class_features() const { return class_feature_library; }

const StorageContentLibrary<Choosable>& Content::get_choosables() const { return choosable_library; }

std::optional<EffectsProviderVariant> Content::get_effects_provider(const std::string& name) const {
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

void Content::set_subgroup(const std::string& group_name, const std::string& subgroup_name) {
    groups.set_subgroup(group_name, subgroup_name);
}

void Content::set_subgroups(const std::string& group_name, std::set<std::string>&& subgroup_names) {
    groups.set_subgroups(group_name, std::move(subgroup_names));
}

void Content::add_group_member(const std::string& group_name, const std::string& value) {
    groups.add(group_name, value);
}

void Content::add_group_members(const std::string& group_name, std::set<std::string>&& values) {
    groups.add(group_name, std::move(values));
}

OptCRef<Character> Content::add_character(Character&& character) {
    OptCRef<Character> inserted_character = character_libary.add(std::move(character));
    if (inserted_character.has_value()) {
        for (const Feature& feature : inserted_character.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_character;
}

OptCRef<Class> Content::add_class(Class&& cls) {
    OptCRef<Class> inserted_class = class_library.add(std::move(cls));
    if (inserted_class.has_value()) {
        for (const ClassFeature& feature : inserted_class.value().get().get_features()) {
            class_feature_library.add(feature);
        }
    }
    return inserted_class;
}

OptCRef<Subclass> Content::add_subclass(Subclass&& subclass) {
    OptCRef<Subclass> inserted_subclass = subclass_library.add(std::move(subclass));
    if (subclass_library.add(std::move(subclass))) {
        for (const ClassFeature& class_feature : inserted_subclass.value().get().get_features()) {
            class_feature_library.add(class_feature);
        }
    }
    return inserted_subclass;
}

OptCRef<Species> Content::add_species(Species&& species) {
    OptCRef<Species> inserted_species = species_library.add(std::move(species));
    if (inserted_species.has_value()) {
        for (const Feature& feature : inserted_species.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_species;
}

OptCRef<Subspecies> Content::add_subspecies(Subspecies&& subspecies) {
    OptCRef<Subspecies> inserted_subspecies = subspecies_library.add(std::move(subspecies));
    if (inserted_subspecies.has_value()) {
        for (const Feature& feature : inserted_subspecies.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_subspecies;
}

OptCRef<Item> Content::add_item(Item&& item) { return item_library.add(std::move(item)); }

OptCRef<Spell> Content::add_spell(Spell&& spell) { return spell_library.add(std::move(spell)); }

OptCRef<Choosable> Content::add_choosable(Choosable&& choosable) {
    OptCRef<Choosable> inserted_choosable = choosable_library.add(std::move(choosable));
    if (inserted_choosable.has_value()) {
        const Choosable& choosable = inserted_choosable.value().get();
        const std::string name = choosable.get_name();
        const std::string type_name = choosable.get_type();
        groups.add(type_name, name);
    }
    return inserted_choosable;
}

OptCRef<Character> Content::add_character_result(CreateResult<Character>&& character) {
    OptCRef<Character> inserted_character = character_libary.add_result(std::move(character));
    if (inserted_character.has_value()) {
        for (const Feature& feature : inserted_character.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_character;
}

OptCRef<Class> Content::add_class_result(CreateResult<Class>&& cls) {
    OptCRef<Class> inserted_class = class_library.add_result(std::move(cls));
    if (inserted_class.has_value()) {
        for (const ClassFeature& feature : inserted_class.value().get().get_features()) {
            class_feature_library.add(feature);
        }
    }
    return inserted_class;
}

OptCRef<Subclass> Content::add_subclass_result(CreateResult<Subclass>&& subclass) {
    OptCRef<Subclass> inserted_subclass = subclass_library.add_result(std::move(subclass));
    if (subclass_library.add_result(std::move(subclass))) {
        for (const ClassFeature& class_feature : inserted_subclass.value().get().get_features()) {
            class_feature_library.add(class_feature);
        }
    }
    return inserted_subclass;
}

OptCRef<Species> Content::add_species_result(CreateResult<Species>&& species) {
    OptCRef<Species> inserted_species = species_library.add_result(std::move(species));
    if (inserted_species.has_value()) {
        for (const Feature& feature : inserted_species.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_species;
}

OptCRef<Subspecies> Content::add_subspecies_result(CreateResult<Subspecies>&& subspecies) {
    OptCRef<Subspecies> inserted_subspecies = subspecies_library.add_result(std::move(subspecies));
    if (inserted_subspecies.has_value()) {
        for (const Feature& feature : inserted_subspecies.value().get().get_features()) {
            feature_library.add(feature);
        }
    }
    return inserted_subspecies;
}

OptCRef<Item> Content::add_item_result(CreateResult<Item>&& item) { return item_library.add_result(std::move(item)); }

OptCRef<Spell> Content::add_spell_result(CreateResult<Spell>&& spell) {
    return spell_library.add_result(std::move(spell));
}

OptCRef<Choosable> Content::add_choosable_result(CreateResult<Choosable>&& choosable) {
    OptCRef<Choosable> inserted_choosable = choosable_library.add_result(std::move(choosable));
    if (inserted_choosable.has_value()) {
        const Choosable& choosable = inserted_choosable.value().get();
        const std::string name = choosable.get_name();
        const std::string type_name = choosable.get_type();
        groups.add(type_name, name);
    }
    return inserted_choosable;
}

} // namespace dnd
