#include "dnd_config.hpp"

#include "display_visitor.hpp"

#include "core/content_visitors/content_visitor.hpp"
#include "core/models/character.hpp"
#include "core/models/character_class.hpp"
#include "core/models/character_race.hpp"
#include "core/models/character_subclass.hpp"
#include "core/models/character_subrace.hpp"
#include "core/models/effect_holder/choosable.hpp"
#include "core/models/effect_holder/feature.hpp"
#include "core/models/item.hpp"
#include "core/models/spell.hpp"

void dnd::DisplayVisitor::visit(const Character* character_ptr) {
    // TODO
}

void dnd::DisplayVisitor::visit(const CharacterClass* character_class_ptr) {
    // TODO
}

void dnd::DisplayVisitor::visit(const CharacterSubclass* character_subclass_ptr) {
    // TODO
}

void dnd::DisplayVisitor::visit(const CharacterRace* character_race_ptr) {
    // TODO
}

void dnd::DisplayVisitor::visit(const CharacterSubrace* character_subrace_ptr) {
    // TODO
}

void dnd::DisplayVisitor::visit(const Item* item_ptr) {
    // TODO
}

void dnd::DisplayVisitor::visit(const Spell* spell_ptr) {
    // TODO
}

void dnd::DisplayVisitor::visit(const Feature* feature_ptr) {
    // TODO
}

void dnd::DisplayVisitor::visit(const Choosable* choosable_ptr) {
    // TODO
}
