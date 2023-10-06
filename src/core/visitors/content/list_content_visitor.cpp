#include <dnd_config.hpp>

#include "list_content_visitor.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/feature/choosable.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/visitors/content/content_visitor.hpp>

void dnd::ListContentVisitor::reserve(size_t size) { string_list.reserve(size); }

std::vector<std::string> dnd::ListContentVisitor::get_list() { return std::move(string_list); }

void dnd::ListContentVisitor::visit(const Character& character) {
    string_list.emplace_back(fmt::format(
        "{} [CHARACTER] : Level {} {} {}", character.get_name(), character.get_progression().get_level(),
        character.get_basis().get_class()->get_name(), character.get_basis().get_race()->get_name()
    ));
}

void dnd::ListContentVisitor::visit(const CharacterClass& character_class) {
    string_list.emplace_back(fmt::format("{} [CLASS]", character_class.get_name()));
}

void dnd::ListContentVisitor::visit(const CharacterSubclass& character_subclass) {
    string_list.emplace_back(
        fmt::format("{} [{} SUBCLASS]", character_subclass.get_name(), character_subclass.get_class()->get_name())
    );
}
void dnd::ListContentVisitor::visit(const CharacterRace& character_race) {
    string_list.emplace_back(fmt::format("{} [RACE]", character_race.get_name()));
}

void dnd::ListContentVisitor::visit(const CharacterSubrace& character_subrace) {
    string_list.emplace_back(
        fmt::format("{} [{} SUBRACE]", character_subrace.get_name(), character_subrace.get_race()->get_name())
    );
}

void dnd::ListContentVisitor::visit(const Item& item) {
    if (item.requires_attunement()) {
        string_list.emplace_back(fmt::format("{} [ITEM] requires attunement", item.get_name()));
    } else {
        string_list.emplace_back(fmt::format("{} [ITEM] no attunement", item.get_name()));
    }
}

void dnd::ListContentVisitor::visit(const Spell& spell) {
    string_list.emplace_back(fmt::format("{} [SPELL] : {}", spell.get_name(), spell.get_type().short_str()));
}

void dnd::ListContentVisitor::visit(const Feature& feature) {
    string_list.emplace_back(
        fmt::format("{} [FEATURE] : {}", feature.get_name(), feature.get_source_info().get_beautified_source_path())
    );
}

void dnd::ListContentVisitor::visit(const Choosable& choosable) {
    string_list.emplace_back(fmt::format(
        "{} [CHOOSABLE] : {}", choosable.get_name(), choosable.get_source_info().get_beautified_source_path()
    ));
}
