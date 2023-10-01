#include <dnd_config.hpp>

#include "content.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

#include <fmt/format.h>

#include <core/referencing_content_library.hpp>
#include <core/storage_content_library.hpp>

bool dnd::Content::empty() const {
    return characters.empty() && character_classes.empty() && character_subclasses.empty() && character_races.empty()
           && character_subraces.empty() && items.empty() && spells.empty();
}

std::string dnd::Content::status() const {
    return fmt::format(
        "{}\n=== Items ===\nitems parsed: {}\n=== Spells ===\nspells parsed: {}\n=== Character Races ===\ncharacter "
        "races parsed: {}\ncharacter subraces parsed: {}\n=== Character Classes ===\ncharacter classes parsed: "
        "{}\ncharacter subclasses parsed: {}\n=== Characters ===\ncharacters parsed: {}",
        groups.status(), items.size(), spells.size(), character_races.size(), character_subraces.size(),
        character_classes.size(), character_subclasses.size(), characters.size()
    );
}

const dnd::Groups& dnd::Content::get_groups() const { return groups; }

const dnd::StorageContentLibrary<dnd::Character>& dnd::Content::get_characters() const { return characters; }

const dnd::StorageContentLibrary<dnd::CharacterClass>& dnd::Content::get_character_classes() const {
    return character_classes;
}

const dnd::StorageContentLibrary<dnd::CharacterSubclass>& dnd::Content::get_character_subclasses() const {
    return character_subclasses;
}

const dnd::StorageContentLibrary<dnd::CharacterRace>& dnd::Content::get_character_races() const {
    return character_races;
}

const dnd::StorageContentLibrary<dnd::CharacterSubrace>& dnd::Content::get_character_subraces() const {
    return character_subraces;
}

const dnd::StorageContentLibrary<dnd::Item>& dnd::Content::get_items() const { return items; }

const dnd::StorageContentLibrary<dnd::Spell>& dnd::Content::get_spells() const { return spells; }

const dnd::ReferencingContentLibrary<dnd::Feature>& dnd::Content::get_features() const { return features; }

const dnd::StorageContentLibrary<dnd::Choosable>& dnd::Content::get_choosables() const { return choosables; }

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

bool dnd::Content::add_character(dnd::Character&& character) {
    const std::string name = character.get_name();
    if (characters.add(std::move(character))) {
        for (const auto& feature : characters.get(name).get_features()) {
            features.add(feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_character_class(dnd::CharacterClass&& character_class) {
    const std::string name = character_class.get_name();
    if (character_classes.add(std::move(character_class))) {
        for (const auto& feature : character_classes.get(name).get_features()) {
            features.add(feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_character_subclass(dnd::CharacterSubclass&& character_subclass) {
    const std::string name = character_subclass.get_name();
    if (character_subclasses.add(std::move(character_subclass))) {
        for (const auto& feature : character_subclasses.get(name).get_features()) {
            features.add(feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_character_race(dnd::CharacterRace&& character_race) {
    const std::string name = character_race.get_name();
    if (character_races.add(std::move(character_race))) {
        for (const auto& feature : character_races.get(name).get_features()) {
            features.add(feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_character_subrace(dnd::CharacterSubrace&& character_subrace) {
    const std::string name = character_subrace.get_name();
    if (character_subraces.add(std::move(character_subrace))) {
        for (const auto& feature : character_subraces.get(name).get_features()) {
            features.add(feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_item(dnd::Item&& item) { return items.add(std::move(item)); }

bool dnd::Content::add_spell(dnd::Spell&& spell) { return spells.add(std::move(spell)); }

bool dnd::Content::add_choosable(dnd::Choosable&& choosable) {
    const std::string name = choosable.get_name();
    const std::string type_name = choosable.get_type();
    if (choosables.add(std::move(choosable))) {
        groups.add(type_name, name);
        return true;
    }
    return false;
}
