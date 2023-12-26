#include <dnd_config.hpp>

#include "collect_open_tabs_visitor.hpp"

#include <utility>

#include <nlohmann/json.hpp>

#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_species/character_species.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subspecies/character_subspecies.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/visitors/content/content_visitor.hpp>

nlohmann::json dnd::CollectOpenTabsVisitor::get_open_tabs() { return std::move(open_tabs_json); }

void dnd::CollectOpenTabsVisitor::operator()(const Character& character) {
    if (!open_tabs_json.contains("character")) {
        open_tabs_json["character"] = nlohmann::json::array();
    }
    open_tabs_json["character"].push_back(character.get_name());
}

void dnd::CollectOpenTabsVisitor::operator()(const CharacterClass& character_class) {
    if (!open_tabs_json.contains("character_class")) {
        open_tabs_json["character_class"] = nlohmann::json::array();
    }
    open_tabs_json["character_class"].push_back(character_class.get_name());
}

void dnd::CollectOpenTabsVisitor::operator()(const CharacterSubclass& character_subclass) {
    if (!open_tabs_json.contains("character_subclass")) {
        open_tabs_json["character_subclass"] = nlohmann::json::array();
    }
    open_tabs_json["character_subclass"].push_back(character_subclass.get_name());
}
void dnd::CollectOpenTabsVisitor::operator()(const CharacterSpecies& character_species) {
    if (!open_tabs_json.contains("character_species")) {
        open_tabs_json["character_species"] = nlohmann::json::array();
    }
    open_tabs_json["character_species"].push_back(character_species.get_name());
}

void dnd::CollectOpenTabsVisitor::operator()(const CharacterSubspecies& character_subspecies) {
    if (!open_tabs_json.contains("character_subspecies")) {
        open_tabs_json["character_subspecies"] = nlohmann::json::array();
    }
    open_tabs_json["character_subspecies"].push_back(character_subspecies.get_name());
}

void dnd::CollectOpenTabsVisitor::operator()(const Item& item) {
    if (!open_tabs_json.contains("item")) {
        open_tabs_json["item"] = nlohmann::json::array();
    }
    open_tabs_json["item"].push_back(item.get_name());
}

void dnd::CollectOpenTabsVisitor::operator()(const Spell& spell) {
    if (!open_tabs_json.contains("spell")) {
        open_tabs_json["spell"] = nlohmann::json::array();
    }
    open_tabs_json["spell"].push_back(spell.get_name());
}

void dnd::CollectOpenTabsVisitor::operator()(const Feature& feature) {
    if (!open_tabs_json.contains("feature")) {
        open_tabs_json["feature"] = nlohmann::json::array();
    }
    open_tabs_json["feature"].push_back(feature.get_name());
}

void dnd::CollectOpenTabsVisitor::operator()(const Choosable& choosable) {
    if (!open_tabs_json.contains("choosable")) {
        open_tabs_json["choosable"] = nlohmann::json::array();
    }
    open_tabs_json["choosable"].push_back(choosable.get_name());
}
