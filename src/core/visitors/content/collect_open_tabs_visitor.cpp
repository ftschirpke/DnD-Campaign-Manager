#include <dnd_config.hpp>

#include "collect_open_tabs_visitor.hpp"

#include <nlohmann/json.hpp>

#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/visitors/content/content_visitor.hpp>

void dnd::CollectOpenTabsVisitor::visit(const Character& character) {
    if (!open_tabs_json.contains("character")) {
        open_tabs_json["character"] = nlohmann::json::array();
    }
    open_tabs_json["character"].push_back(character.get_name());
}

void dnd::CollectOpenTabsVisitor::visit(const CharacterClass& character_class) {
    if (!open_tabs_json.contains("character_class")) {
        open_tabs_json["character_class"] = nlohmann::json::array();
    }
    open_tabs_json["character_class"].push_back(character_class.get_name());
}

void dnd::CollectOpenTabsVisitor::visit(const CharacterSubclass& character_subclass) {
    if (!open_tabs_json.contains("character_subclass")) {
        open_tabs_json["character_subclass"] = nlohmann::json::array();
    }
    open_tabs_json["character_subclass"].push_back(character_subclass.get_name());
}
void dnd::CollectOpenTabsVisitor::visit(const CharacterRace& character_race) {
    if (!open_tabs_json.contains("character_race")) {
        open_tabs_json["character_race"] = nlohmann::json::array();
    }
    open_tabs_json["character_race"].push_back(character_race.get_name());
}

void dnd::CollectOpenTabsVisitor::visit(const CharacterSubrace& character_subrace) {
    if (!open_tabs_json.contains("character_subrace")) {
        open_tabs_json["character_subrace"] = nlohmann::json::array();
    }
    open_tabs_json["character_subrace"].push_back(character_subrace.get_name());
}

void dnd::CollectOpenTabsVisitor::visit(const Item& item) {
    if (!open_tabs_json.contains("item")) {
        open_tabs_json["item"] = nlohmann::json::array();
    }
    open_tabs_json["item"].push_back(item.get_name());
}

void dnd::CollectOpenTabsVisitor::visit(const Spell& spell) {
    if (!open_tabs_json.contains("spell")) {
        open_tabs_json["spell"] = nlohmann::json::array();
    }
    open_tabs_json["spell"].push_back(spell.get_name());
}

void dnd::CollectOpenTabsVisitor::visit(const Feature& feature) {
    if (!open_tabs_json.contains("feature")) {
        open_tabs_json["feature"] = nlohmann::json::array();
    }
    open_tabs_json["feature"].push_back(feature.get_name());
}

void dnd::CollectOpenTabsVisitor::visit(const Choosable& choosable) {
    if (!open_tabs_json.contains("choosable")) {
        open_tabs_json["choosable"] = nlohmann::json::array();
    }
    open_tabs_json["choosable"].push_back(choosable.get_name());
}
