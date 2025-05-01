#include <dnd_config.hpp>

#include "collect_open_tabs_visitor.hpp"

#include <utility>

#include <nlohmann/json.hpp>

#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

nlohmann::json CollectOpenTabsVisitor::get_open_tabs() { return std::move(open_tabs_json); }

void CollectOpenTabsVisitor::operator()(const Character& character) {
    if (!open_tabs_json.contains("character")) {
        open_tabs_json["character"] = nlohmann::json::array();
    }
    open_tabs_json["character"].push_back(character.get_key());
}

void CollectOpenTabsVisitor::operator()(const Class& cls) {
    if (!open_tabs_json.contains("class")) {
        open_tabs_json["class"] = nlohmann::json::array();
    }
    open_tabs_json["class"].push_back(cls.get_key());
}

void CollectOpenTabsVisitor::operator()(const Subclass& subclass) {
    if (!open_tabs_json.contains("subclass")) {
        open_tabs_json["subclass"] = nlohmann::json::array();
    }
    open_tabs_json["subclass"].push_back(subclass.get_key());
}
void CollectOpenTabsVisitor::operator()(const Species& species) {
    if (!open_tabs_json.contains("species")) {
        open_tabs_json["species"] = nlohmann::json::array();
    }
    open_tabs_json["species"].push_back(species.get_key());
}

void CollectOpenTabsVisitor::operator()(const Subspecies& subspecies) {
    if (!open_tabs_json.contains("subspecies")) {
        open_tabs_json["subspecies"] = nlohmann::json::array();
    }
    open_tabs_json["subspecies"].push_back(subspecies.get_key());
}

void CollectOpenTabsVisitor::operator()(const Item& item) {
    if (!open_tabs_json.contains("item")) {
        open_tabs_json["item"] = nlohmann::json::array();
    }
    open_tabs_json["item"].push_back(item.get_key());
}

void CollectOpenTabsVisitor::operator()(const Spell& spell) {
    if (!open_tabs_json.contains("spell")) {
        open_tabs_json["spell"] = nlohmann::json::array();
    }
    open_tabs_json["spell"].push_back(spell.get_key());
}

void CollectOpenTabsVisitor::operator()(const Feature& feature) {
    if (!open_tabs_json.contains("feature")) {
        open_tabs_json["feature"] = nlohmann::json::array();
    }
    open_tabs_json["feature"].push_back(feature.get_key());
}

void CollectOpenTabsVisitor::operator()(const ClassFeature& class_feature) {
    if (!open_tabs_json.contains("class_feature")) {
        open_tabs_json["class_feature"] = nlohmann::json::array();
    }
    open_tabs_json["class_feature"].push_back(class_feature.get_name());
}

void CollectOpenTabsVisitor::operator()(const Choosable& choosable) {
    if (!open_tabs_json.contains("choosable")) {
        open_tabs_json["choosable"] = nlohmann::json::array();
    }
    open_tabs_json["choosable"].push_back(choosable.get_key());
}

} // namespace dnd
