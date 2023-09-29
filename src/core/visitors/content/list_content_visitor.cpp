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

void dnd::ListContentVisitor::visit(const Character* character_ptr) {
    string_list.emplace_back(fmt::format(
        "{} [CHARACTER] : Level {} {} {}", character_ptr->get_name(), character_ptr->get_progression().get_level(),
        character_ptr->get_basis().get_class()->get_name(), character_ptr->get_basis().get_race()->get_name()
    ));
}

void dnd::ListContentVisitor::visit(const CharacterClass* character_class_ptr) {
    string_list.emplace_back(fmt::format("{} [CLASS]", character_class_ptr->get_name()));
}

void dnd::ListContentVisitor::visit(const CharacterSubclass* character_subclass_ptr) {
    string_list.emplace_back(fmt::format(
        "{} [{} SUBCLASS]", character_subclass_ptr->get_name(), character_subclass_ptr->get_class()->get_name()
    ));
}
void dnd::ListContentVisitor::visit(const CharacterRace* character_race_ptr) {
    string_list.emplace_back(fmt::format("{} [RACE]", character_race_ptr->get_name()));
}

void dnd::ListContentVisitor::visit(const CharacterSubrace* character_subrace_ptr) {
    string_list.emplace_back(
        fmt::format("{} [{} SUBRACE]", character_subrace_ptr->get_name(), character_subrace_ptr->get_race()->get_name())
    );
}

void dnd::ListContentVisitor::visit(const Item* item_ptr) {
    if (item_ptr->requires_attunement()) {
        string_list.emplace_back(fmt::format("{} [ITEM] requires attunement", item_ptr->get_name()));
    } else {
        string_list.emplace_back(fmt::format("{} [ITEM] no attunement", item_ptr->get_name()));
    }
}

void dnd::ListContentVisitor::visit(const Spell* spell_ptr) {
    string_list.emplace_back(fmt::format("{} [SPELL] : {}", spell_ptr->get_name(), spell_ptr->get_type().short_str()));
}

void dnd::ListContentVisitor::visit(const Feature* feature_ptr) {
    string_list.emplace_back(fmt::format(
        "{} [FEATURE] : {}", feature_ptr->get_name(), feature_ptr->get_source_info().get_beautified_source_path()
    ));
}

void dnd::ListContentVisitor::visit(const Choosable* choosable_ptr) {
    string_list.emplace_back(fmt::format(
        "{} [CHOOSABLE] : {}", choosable_ptr->get_name(), choosable_ptr->get_source_info().get_beautified_source_path()
    ));
}
