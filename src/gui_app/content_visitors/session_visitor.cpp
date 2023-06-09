#include <dnd_config.hpp>

#include "session_visitor.hpp"

#include <nlohmann/json.hpp>

#include <core/content_visitors/content_visitor.hpp>
#include <core/models/character.hpp>
#include <core/models/character_class.hpp>
#include <core/models/character_race.hpp>
#include <core/models/character_subclass.hpp>
#include <core/models/character_subrace.hpp>
#include <core/models/effect_holder/choosable.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/item.hpp>
#include <core/models/spell.hpp>

void dnd::SessionVisitor::visit(const Character* character_ptr) {
    if (!open_tabs_json.contains("character")) {
        open_tabs_json["character"] = nlohmann::json::array();
    }
    open_tabs_json["character"].push_back(character_ptr->get_name());
}

void dnd::SessionVisitor::visit(const CharacterClass* character_class_ptr) {
    if (!open_tabs_json.contains("character_class")) {
        open_tabs_json["character_class"] = nlohmann::json::array();
    }
    open_tabs_json["character_class"].push_back(character_class_ptr->get_name());
}

void dnd::SessionVisitor::visit(const CharacterSubclass* character_subclass_ptr) {
    if (!open_tabs_json.contains("character_subclass")) {
        open_tabs_json["character_subclass"] = nlohmann::json::array();
    }
    open_tabs_json["character_subclass"].push_back(character_subclass_ptr->get_name());
}
void dnd::SessionVisitor::visit(const CharacterRace* character_race_ptr) {
    if (!open_tabs_json.contains("character_race")) {
        open_tabs_json["character_race"] = nlohmann::json::array();
    }
    open_tabs_json["character_race"].push_back(character_race_ptr->get_name());
}

void dnd::SessionVisitor::visit(const CharacterSubrace* character_subrace_ptr) {
    if (!open_tabs_json.contains("character_subrace")) {
        open_tabs_json["character_subrace"] = nlohmann::json::array();
    }
    open_tabs_json["character_subrace"].push_back(character_subrace_ptr->get_name());
}

void dnd::SessionVisitor::visit(const Item* item_ptr) {
    if (!open_tabs_json.contains("item")) {
        open_tabs_json["item"] = nlohmann::json::array();
    }
    open_tabs_json["item"].push_back(item_ptr->get_name());
}

void dnd::SessionVisitor::visit(const Spell* spell_ptr) {
    if (!open_tabs_json.contains("spell")) {
        open_tabs_json["spell"] = nlohmann::json::array();
    }
    open_tabs_json["spell"].push_back(spell_ptr->get_name());
}

void dnd::SessionVisitor::visit(const Feature* feature_ptr) {
    if (!open_tabs_json.contains("feature")) {
        open_tabs_json["feature"] = nlohmann::json::array();
    }
    open_tabs_json["feature"].push_back(feature_ptr->get_name());
}

void dnd::SessionVisitor::visit(const Choosable* choosable_ptr) {
    if (!open_tabs_json.contains("choosable")) {
        open_tabs_json["choosable"] = nlohmann::json::array();
    }
    open_tabs_json["choosable"].push_back(choosable_ptr->get_name());
}
