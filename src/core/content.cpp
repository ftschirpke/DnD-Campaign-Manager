#include <dnd_config.hpp>

#include "content.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

#include <core/content_library.hpp>

bool dnd::Content::empty() const {
    return characters.empty() && character_classes.empty() && character_subclasses.empty() && character_races.empty()
           && character_subraces.empty() && items.empty() && spells.empty();
}

std::string dnd::Content::status() const {
    std::stringstream sstr;
    sstr << groups.status();
    sstr << "=== Items ===\n";
    sstr << "items parsed: " << items.size() << '\n';
    sstr << "=== Spells ===\n";
    sstr << "spells parsed: " << spells.size() << '\n';
    sstr << "=== Character Races ===\n";
    sstr << "character races parsed: " << character_races.size() << '\n';
    sstr << "character subraces parsed: " << character_subraces.size() << '\n';
    sstr << "=== Character Classes ===\n";
    sstr << "character classes parsed: " << character_classes.size() << '\n';
    sstr << "character subclasses parsed: " << character_subclasses.size() << '\n';
    sstr << "=== Characters ===\n";
    sstr << "characters parsed: " << characters.size() << '\n';
    return sstr.str();
}

const dnd::Groups& dnd::Content::get_groups() const { return groups; }

const dnd::StorageContentLibrary<dnd::Character>& dnd::Content::get_characters() const { return characters; }

const dnd::StorageContentLibrary<const dnd::CharacterClass>& dnd::Content::get_character_classes() const {
    return character_classes;
}

const dnd::StorageContentLibrary<const dnd::CharacterSubclass>& dnd::Content::get_character_subclasses() const {
    return character_subclasses;
}

const dnd::StorageContentLibrary<const dnd::CharacterRace>& dnd::Content::get_character_races() const {
    return character_races;
}

const dnd::StorageContentLibrary<const dnd::CharacterSubrace>& dnd::Content::get_character_subraces() const {
    return character_subraces;
}

const dnd::StorageContentLibrary<const dnd::Item>& dnd::Content::get_items() const { return items; }

const dnd::StorageContentLibrary<const dnd::Spell>& dnd::Content::get_spells() const { return spells; }

const dnd::ReferencingContentLibrary<const dnd::Feature>& dnd::Content::get_features() const { return features; }

const dnd::StorageContentLibrary<const dnd::ChoosableFeature>& dnd::Content::get_choosable_features() const {
    return choosable_features;
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

bool dnd::Content::add_character(dnd::Character&& character) {
    const std::string name = character.get_name();
    if (characters.add(name, std::move(character))) {
        for (const auto& feature : characters.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_character_class(dnd::CharacterClass&& character_class) {
    const std::string name = character_class.get_name();
    if (character_classes.add(name, std::move(character_class))) {
        for (const auto& feature : character_classes.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_character_subclass(dnd::CharacterSubclass&& character_subclass) {
    const std::string name = character_subclass.get_name();
    if (character_subclasses.add(name, std::move(character_subclass))) {
        for (const auto& feature : character_subclasses.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_character_race(dnd::CharacterRace&& character_race) {
    const std::string name = character_race.get_name();
    if (character_races.add(name, std::move(character_race))) {
        for (const auto& feature : character_races.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_character_subrace(dnd::CharacterSubrace&& character_subrace) {
    const std::string name = character_subrace.get_name();
    if (character_subraces.add(name, std::move(character_subrace))) {
        for (const auto& feature : character_subraces.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::Content::add_item(dnd::Item&& item) { return items.add(item.get_name(), std::move(item)); }

bool dnd::Content::add_spell(dnd::Spell&& spell) { return spells.add(spell.get_name(), std::move(spell)); }

bool dnd::Content::add_choosable_feature(dnd::ChoosableFeature&& choosable_feature) {
    const std::string name = choosable_feature.get_name();
    const std::string type_name = choosable_feature.get_type();
    if (choosable_features.add(name, std::move(choosable_feature))) {
        groups.add(type_name, name);
        return true;
    }
    return false;
}
