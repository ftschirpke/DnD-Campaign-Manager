#include "dnd_config.hpp"

#include "list_visitor.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "core/models/character.hpp"
#include "core/models/character_class.hpp"
#include "core/models/character_race.hpp"
#include "core/models/character_subclass.hpp"
#include "core/models/character_subrace.hpp"
#include "core/models/effect_holder/choosable.hpp"
#include "core/models/effect_holder/feature.hpp"
#include "core/models/item.hpp"
#include "core/models/spell.hpp"
#include "core/visitors/visitor.hpp"

void dnd::ListVisitor::visit(const Character* character_ptr) {
    string_list.emplace_back(fmt::format(
        "{} [CHARACTER] : Level {} {} {}", character_ptr->name, character_ptr->getLevel(),
        character_ptr->class_ptr->name, character_ptr->race_ptr->name
    ));
}

void dnd::ListVisitor::visit(const CharacterClass* character_class_ptr) {
    string_list.emplace_back(fmt::format("{} [CLASS]", character_class_ptr->name));
}

void dnd::ListVisitor::visit(const CharacterSubclass* character_subclass_ptr) {
    string_list.emplace_back(
        fmt::format("{} [{} SUBCLASS]", character_subclass_ptr->name, character_subclass_ptr->class_name)
    );
}
void dnd::ListVisitor::visit(const CharacterRace* character_race_ptr) {
    string_list.emplace_back(fmt::format("{} [RACE]", character_race_ptr->name));
}

void dnd::ListVisitor::visit(const CharacterSubrace* character_subrace_ptr) {
    string_list.emplace_back(
        fmt::format("{} [{} SUBRACE]", character_subrace_ptr->name, character_subrace_ptr->race_name)
    );
}

void dnd::ListVisitor::visit(const Item* item_ptr) {
    if (item_ptr->requires_attunement) {
        string_list.emplace_back(fmt::format("{} [ITEM] requires attunement", item_ptr->name));
    } else {
        string_list.emplace_back(fmt::format("{} [ITEM] no attunement", item_ptr->name));
    }
}

void dnd::ListVisitor::visit(const Spell* spell_ptr) {
    string_list.emplace_back(fmt::format("{} [SPELL] : {}", spell_ptr->name, spell_ptr->type.short_str()));
}

void dnd::ListVisitor::visit(const Feature* feature_ptr) {
    std::filesystem::path feature_path = std::filesystem::relative(
        feature_ptr->source_file_path, feature_ptr->source_file_path.parent_path().parent_path()
    );
    feature_path.replace_extension("");
    string_list.emplace_back(fmt::format("{} [FEATURE] : {}", feature_ptr->name, feature_path.string()));
}

void dnd::ListVisitor::visit(const Choosable* choosable_ptr) {
    std::filesystem::path choosable_path = std::filesystem::relative(
        choosable_ptr->source_file_path, choosable_ptr->source_file_path.parent_path().parent_path()
    );
    choosable_path.replace_extension("");
    string_list.emplace_back(fmt::format("{} [CHOOSABLE] : {}", choosable_ptr->name, choosable_path.string()));
}