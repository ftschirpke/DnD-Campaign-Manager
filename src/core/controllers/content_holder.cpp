#include <dnd_config.hpp>

#include "content_holder.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

#include <core/controllers/content_library.hpp>

bool dnd::ContentHolder::empty() const {
    return characters.empty() && character_classes.empty() && character_subclasses.empty() && character_races.empty()
           && character_subraces.empty() && items.empty() && spells.empty();
}

std::string dnd::ContentHolder::status() const {
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

const dnd::Groups& dnd::ContentHolder::get_groups() const { return groups; }

const dnd::StorageContentLibrary<dnd::Character>& dnd::ContentHolder::get_characters() const { return characters; }

const dnd::StorageContentLibrary<const dnd::CharacterClass>& dnd::ContentHolder::get_character_classes() const {
    return character_classes;
}

const dnd::StorageContentLibrary<const dnd::CharacterSubclass>& dnd::ContentHolder::get_character_subclasses() const {
    return character_subclasses;
}

const dnd::StorageContentLibrary<const dnd::CharacterRace>& dnd::ContentHolder::get_character_races() const {
    return character_races;
}

const dnd::StorageContentLibrary<const dnd::CharacterSubrace>& dnd::ContentHolder::get_character_subraces() const {
    return character_subraces;
}

const dnd::StorageContentLibrary<const dnd::Item>& dnd::ContentHolder::get_items() const { return items; }

const dnd::StorageContentLibrary<const dnd::Spell>& dnd::ContentHolder::get_spells() const { return spells; }

const dnd::ReferencingContentLibrary<const dnd::Feature>& dnd::ContentHolder::get_features() const { return features; }

const dnd::StorageContentLibrary<const dnd::ChoosableFeature>& dnd::ContentHolder::get_choosable_features() const {
    return choosable_features;
}

void dnd::ContentHolder::set_subgroup(const std::string& group_name, const std::string& subgroup_name) {
    groups.set_subgroup(group_name, subgroup_name);
}

void dnd::ContentHolder::set_subgroups(const std::string& group_name, std::set<std::string>&& subgroup_names) {
    groups.set_subgroups(group_name, std::move(subgroup_names));
}

void dnd::ContentHolder::add_group_member(const std::string& group_name, const std::string& value) {
    groups.add(group_name, value);
}

void dnd::ContentHolder::add_group_members(const std::string& group_name, std::set<std::string>&& values) {
    groups.add(group_name, std::move(values));
}

bool dnd::ContentHolder::add_character(dnd::Character&& character) {
    const std::string name = character.get_name();
    if (characters.add(name, std::move(character))) {
        for (const auto& feature : characters.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::ContentHolder::add_character_class(dnd::CharacterClass&& character_class) {
    const std::string name = character_class.get_name();
    if (character_classes.add(name, std::move(character_class))) {
        for (const auto& feature : character_classes.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::ContentHolder::add_character_subclass(dnd::CharacterSubclass&& character_subclass) {
    const std::string name = character_subclass.get_name();
    if (character_subclasses.add(name, std::move(character_subclass))) {
        for (const auto& feature : character_subclasses.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::ContentHolder::add_character_race(dnd::CharacterRace&& character_race) {
    const std::string name = character_race.get_name();
    if (character_races.add(name, std::move(character_race))) {
        for (const auto& feature : character_races.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::ContentHolder::add_character_subrace(dnd::CharacterSubrace&& character_subrace) {
    const std::string name = character_subrace.get_name();
    if (character_subraces.add(name, std::move(character_subrace))) {
        for (const auto& feature : character_subraces.get(name).get_features()) {
            features.add(feature.get_name(), &feature);
        }
        return true;
    }
    return false;
}

bool dnd::ContentHolder::add_item(dnd::Item&& item) { return items.add(item.get_name(), std::move(item)); }

bool dnd::ContentHolder::add_spell(dnd::Spell&& spell) { return spells.add(spell.get_name(), std::move(spell)); }

bool dnd::ContentHolder::add_choosable_feature(dnd::ChoosableFeature&& choosable_feature) {
    const std::string name = choosable_feature.get_name();
    const std::string type_name = choosable_feature.get_type();
    if (choosable_features.add(name, std::move(choosable_feature))) {
        groups.add(type_name, name);
        return true;
    }
    return false;
}
