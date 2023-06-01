#include "dnd_config.hpp"

#include "list_visitor.hpp"

#include <string>
#include <vector>

#include <fmt/format.h>

#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_subclass.hpp"
#include "models/character_subrace.hpp"
#include "models/effect_holder/choosable.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/item.hpp"
#include "models/spell.hpp"
#include "visitors/visitor.hpp"

void dnd::ListVisitor::visit(const Character* character_ptr) {
    string_list.emplace_back(fmt::format(
        "%s (Character) - %s - Level %d %s", character_ptr->name, character_ptr->race_ptr->name,
        character_ptr->getLevel(), character_ptr->class_ptr->name
    ));
}

void dnd::ListVisitor::visit(const CharacterClass* character_class_ptr) {
    string_list.emplace_back(fmt::format("%s (Class)", character_class_ptr->name));
}

void dnd::ListVisitor::visit(const CharacterSubclass* character_subclass_ptr) {
    string_list.emplace_back(
        fmt::format("%s (Subclass %s)", character_subclass_ptr->name, character_subclass_ptr->class_name)
    );
}
void dnd::ListVisitor::visit(const CharacterRace* character_race_ptr) {
    string_list.emplace_back(fmt::format("%s (Race)", character_race_ptr->name));
}

void dnd::ListVisitor::visit(const CharacterSubrace* character_subrace_ptr) {
    string_list.emplace_back(
        fmt::format("%s (Subrace %s)", character_subrace_ptr->name, character_subrace_ptr->race_name)
    );
}

void dnd::ListVisitor::visit(const Item* item_ptr) {
    string_list.emplace_back(fmt::format("%s (Item)", item_ptr->name));
}

void dnd::ListVisitor::visit(const Spell* spell_ptr) {
    string_list.emplace_back(fmt::format("%s (Spell)", spell_ptr->name));
}

void dnd::ListVisitor::visit(const Feature* feature_ptr) {
    string_list.emplace_back(fmt::format("%s (Feature)", feature_ptr->name));
}

void dnd::ListVisitor::visit(const Choosable* choosable_ptr) {
    string_list.emplace_back(fmt::format("%s (Choosable)", choosable_ptr->name));
}
