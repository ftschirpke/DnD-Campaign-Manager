#include <dnd_config.hpp>

#include "list_content_visitor.hpp"

#include <string>
#include <vector>

#include <fmt/format.h>

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

void ListContentVisitor::reserve(size_t size) { string_list.reserve(size); }

const std::vector<std::string>& ListContentVisitor::get_list() const { return string_list; }

std::vector<std::string> ListContentVisitor::get_list() { return std::move(string_list); }

void ListContentVisitor::clear_list() { string_list.clear(); }

void ListContentVisitor::operator()(const Character& character) {
    string_list.push_back(fmt::format(
        "{} ({}) [CHARACTER] : Level {} {} {}##{}", character.get_name(), character.get_source_info().name,
        character.get_progression().get_level(), character.get_feature_providers().get_class().get_name(),
        character.get_feature_providers().get_species().get_name(), character.get_key()
    ));
}

void ListContentVisitor::operator()(const Class& cls) {
    string_list.push_back(fmt::format("{} ({}) [CLASS]##{}", cls.get_name(), cls.get_source_info().name, cls.get_key())
    );
}

void ListContentVisitor::operator()(const Subclass& subclass) {
    const Class& cls = subclass.get_class();
    string_list.push_back(fmt::format(
        "{} ({}) [{} SUBCLASS]##{}", subclass.get_name(), subclass.get_source_info().name, cls.get_name(),
        subclass.get_key()
    ));
}
void ListContentVisitor::operator()(const Species& species) {
    string_list.push_back(
        fmt::format("{} ({}) [SPECIES]##{}", species.get_name(), species.get_source_info().name, species.get_key())
    );
}

void ListContentVisitor::operator()(const Subspecies& subspecies) {
    const Species& species = subspecies.get_species();
    string_list.push_back(fmt::format(
        "{} ({}) [{} SUBSPECIES]##{}", subspecies.get_name(), subspecies.get_source_info().name, species.get_name(),
        subspecies.get_key()
    ));
}

void ListContentVisitor::operator()(const Item& item) {
    if (item.requires_attunement()) {
        string_list.push_back(fmt::format(
            "{} ({}) [ITEM] requires attunement##{}", item.get_name(), item.get_source_info().name, item.get_key()
        ));
    } else {
        string_list.push_back(fmt::format(
            "{} ({}) [ITEM] no attunement##{}", item.get_name(), item.get_source_info().name, item.get_key()
        ));
    }
}

void ListContentVisitor::operator()(const Spell& spell) {
    string_list.push_back(fmt::format(
        "{} ({}) [SPELL] : {}##{}", spell.get_name(), spell.get_source_info().name, spell.get_type().short_str(),
        spell.get_key()
    ));
}

void ListContentVisitor::operator()(const Feature& feature) {
    string_list.push_back(fmt::format(
        "{} ({}) [FEATURE] : {}##{}", feature.get_name(), feature.get_source_info().name,
        feature.get_source_info().name, feature.get_key()
    ));
}

void ListContentVisitor::operator()(const ClassFeature& class_feature) {
    string_list.push_back(fmt::format(
        "{} ({}) [CLASS FEATURE] : {}##{}", class_feature.get_name(), class_feature.get_source_info().name,
        class_feature.get_source_info().name, class_feature.get_key()
    ));
}

void ListContentVisitor::operator()(const SubclassFeature& subclass_feature) {
    string_list.push_back(fmt::format(
        "{} ({}) [SUBCLASS FEATURE] : {}##{}", subclass_feature.get_name(), subclass_feature.get_source_info().name,
        subclass_feature.get_source_info().name, subclass_feature.get_key()
    ));
}


void ListContentVisitor::operator()(const Choosable& choosable) {
    string_list.push_back(fmt::format(
        "{} ({}) [CHOOSABLE] : {}##{}", choosable.get_name(), choosable.get_source_info().name,
        choosable.get_source_info().name, choosable.get_key()
    ));
}

} // namespace dnd
