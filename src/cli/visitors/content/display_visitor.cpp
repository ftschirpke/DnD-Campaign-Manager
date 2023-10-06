#include <dnd_config.hpp>

#include "display_visitor.hpp"

#include <cassert>
#include <string>

#include <fmt/format.h>

#include <cli/output/command_line_output.hpp>
#include <core/basic_mechanics/dice.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/feature/choosable.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spell/spell.hpp>
#include <core/visitors/content/content_visitor.hpp>

constexpr const char* separator = "--------------------------------------------------------------------------------";

static void display_source_info(dnd::CommandLineOutput& output, const dnd::SourceInfo& source_info) {
    output.formatted_text(
        "Source: {} / {} / {}", source_info.get_source_group_name(), source_info.get_source_type_name(),
        source_info.get_source_name()
    );
}

static void list_features(dnd::CommandLineOutput& output, const std::vector<dnd::Feature>& features) {
    if (features.empty()) {
        return;
    }
    output.text("Features:");
    for (const dnd::Feature& feature : features) {
        output.text(separator);
        output.text(feature.get_name());
        output.text("Description:");
        output.text(feature.get_description());
    }
}

void dnd::DisplayVisitor::visit(const dnd::Character& character) {
    output.text(character.get_name());
    output.text("Type: Character");
    display_source_info(output, character.get_source_info());
    output.formatted_text("Level: {}", character.get_progression().get_level());
    output.formatted_text("XP: {}", character.get_progression().get_xp());

    const CharacterRace* race_ptr = character.get_basis().get_race();
    assert(race_ptr != nullptr);
    output.formatted_text("Race: {}", race_ptr->get_name());

    const CharacterSubrace* subrace_ptr = character.get_basis().get_subrace();
    if (subrace_ptr != nullptr) {
        output.formatted_text("Subrace: {}", subrace_ptr->get_name());
    }

    const CharacterClass* class_ptr = character.get_basis().get_class();
    assert(class_ptr != nullptr);
    output.formatted_text("Class: {}", class_ptr->get_name());

    const CharacterSubclass* subclass_ptr = character.get_basis().get_subclass();
    if (subclass_ptr != nullptr) {
        output.formatted_text("Subclass: {}", subclass_ptr->get_name());
    }

    list_features(output, character.get_features());
}

void dnd::DisplayVisitor::visit(const dnd::CharacterClass& character_class) {
    output.text(character_class.get_name());
    output.text("Type: Class");
    display_source_info(output, character_class.get_source_info());
    output.formatted_text("Hit Die: {}", dice_to_string(character_class.get_hit_dice()));

    std::string feat_level_str = fmt::format(
        "{}", fmt::join(character_class.get_important_levels().get_feat_levels(), ", ")
    );
    output.formatted_text("Feat Levels: {}", feat_level_str);
    output.formatted_text("Subclass Level: {}", character_class.get_important_levels().get_subclass_level());

    list_features(output, character_class.get_features());
}

void dnd::DisplayVisitor::visit(const CharacterSubclass& character_subclass) {
    output.text(character_subclass.get_name());
    output.text("Type: Subclass");
    display_source_info(output, character_subclass.get_source_info());
    output.formatted_text("Class name:", character_subclass.get_class()->get_name());

    list_features(output, character_subclass.get_features());
}

void dnd::DisplayVisitor::visit(const CharacterRace& character_race) {
    output.text(character_race.get_name());
    output.text("Type: Race");
    display_source_info(output, character_race.get_source_info());
    const char* has_subraces_cstr = character_race.has_subraces() ? "yes" : "no";
    output.formatted_text("Has Subraces: {}", has_subraces_cstr);

    list_features(output, character_race.get_features());
}

void dnd::DisplayVisitor::visit(const CharacterSubrace& character_subrace) {
    output.text(character_subrace.get_name());
    output.text("Type: Subrace");
    display_source_info(output, character_subrace.get_source_info());
    output.formatted_text("Race name: {}", character_subrace.get_race()->get_name());

    list_features(output, character_subrace.get_features());
}

void dnd::DisplayVisitor::visit(const Item& item) {
    output.text(item.get_name());
    output.text("Type: Item");
    display_source_info(output, item.get_source_info());
    const char* attunement = item.requires_attunement() ? "required" : "not required";
    output.formatted_text("Attunement {}", attunement);
    output.text("Description:");
    output.text(item.get_description());
    if (!item.get_cosmetic_description().empty()) {
        output.text("Cosmetic Description:");
        output.text(item.get_cosmetic_description());
    }
}

void dnd::DisplayVisitor::visit(const Spell& spell) {
    output.text(spell.get_name());
    output.text("Type: Spell");
    display_source_info(output, spell.get_source_info());
    output.formatted_text("Spell Type: {}", spell.get_type().str());
    output.formatted_text("Casting Time: {}", spell.get_casting_time());
    output.formatted_text("Range: {}", spell.get_range());
    output.formatted_text("Components: {}", spell.get_components().str());
    output.formatted_text("Duration: {}", spell.get_duration());

    output.text("Description:");
    output.text(spell.get_description());
}

void dnd::DisplayVisitor::visit(const Feature& feature) {
    output.text(feature.get_name());
    output.text("Type: Feature");
    display_source_info(output, feature.get_source_info());
    output.text("Description:");
    output.text(feature.get_description());
}

void dnd::DisplayVisitor::visit(const Choosable& choosable) {
    output.text(choosable.get_name());
    output.formatted_text("Type: Choosable Feature - %s", choosable.get_type());
    display_source_info(output, choosable.get_source_info());
    output.text("Description:");
    output.text(choosable.get_description());
}
