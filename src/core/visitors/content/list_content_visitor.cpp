#include <dnd_config.hpp>

#include "list_content_visitor.hpp"

#include <filesystem>
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

void dnd::ListContentVisitor::reserve(size_t size) { string_list.reserve(size); }

std::vector<std::string> dnd::ListContentVisitor::get_list() { return std::move(string_list); }

void dnd::ListContentVisitor::operator()(const Character& character) {
    string_list.emplace_back(fmt::format(
        "{} [CHARACTER] : Level {} {} {}", character.get_name(), character.get_progression().get_level(),
        character.get_basis().get_class().get_name(), character.get_basis().get_species().get_name()
    ));
}

void dnd::ListContentVisitor::operator()(const Class& cls) {
    string_list.emplace_back(fmt::format("{} [CLASS]", cls.get_name()));
}

void dnd::ListContentVisitor::operator()(const Subclass& subclass) {
    string_list.emplace_back(fmt::format("{} [{} SUBCLASS]", subclass.get_name(), subclass.get_class()->get_name()));
}
void dnd::ListContentVisitor::operator()(const Species& species) {
    string_list.emplace_back(fmt::format("{} [RACE]", species.get_name()));
}

void dnd::ListContentVisitor::operator()(const Subspecies& subspecies) {
    string_list.emplace_back(fmt::format("{} [{} SUBRACE]", subspecies.get_name(), subspecies.get_species()->get_name())
    );
}

void dnd::ListContentVisitor::operator()(const Item& item) {
    if (item.requires_attunement()) {
        string_list.emplace_back(fmt::format("{} [ITEM] requires attunement", item.get_name()));
    } else {
        string_list.emplace_back(fmt::format("{} [ITEM] no attunement", item.get_name()));
    }
}

void dnd::ListContentVisitor::operator()(const Spell& spell) {
    string_list.emplace_back(fmt::format("{} [SPELL] : {}", spell.get_name(), spell.get_type().short_str()));
}

void dnd::ListContentVisitor::operator()(const Feature& feature) {
    string_list.emplace_back(
        fmt::format("{} [FEATURE] : {}", feature.get_name(), feature.get_source_info().get_beautified_source_path())
    );
}

void dnd::ListContentVisitor::operator()(const Choosable& choosable) {
    string_list.emplace_back(fmt::format(
        "{} [CHOOSABLE] : {}", choosable.get_name(), choosable.get_source_info().get_beautified_source_path()
    ));
}
