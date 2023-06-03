#include "dnd_config.hpp"

#include "display_visitor.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <imgui/imgui.h>

#include "core/basic_mechanics/dice.hpp"
#include "core/content_visitors/content_visitor.hpp"
#include "core/models/character.hpp"
#include "core/models/character_class.hpp"
#include "core/models/character_race.hpp"
#include "core/models/character_subclass.hpp"
#include "core/models/character_subrace.hpp"
#include "core/models/content_piece.hpp"
#include "core/models/effect_holder/choosable.hpp"
#include "core/models/effect_holder/feature.hpp"
#include "core/models/feature_holder.hpp"
#include "core/models/item.hpp"
#include "core/models/spell.hpp"
#include "core/output/string_formatting/formats/format.hpp"
#include "core/output/string_formatting/string_formatter.hpp"

static const ImVec2 cell_padding = ImVec2(5, 5);
static const ImGuiTableFlags content_table_flags = ImGuiTableFlags_NoBordersInBodyUntilResize;
static const float first_column_width = 150;

static void label(const char* label) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", label);
    ImGui::TableSetColumnIndex(1);
}

static void source(const dnd::ContentPiece* content_piece_ptr) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Source:");
    ImGui::Separator();
    ImGui::TableSetColumnIndex(1);
    ImGui::TextWrapped("%s", content_piece_ptr->source_file_path.string().c_str());
    ImGui::Separator();
}

static void begin_content_table(const dnd::ContentPiece* content_piece_ptr) {
    std::string table_id = content_piece_ptr->name + "_table";
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
    ImGui::BeginTable(table_id.c_str(), 2, content_table_flags);
    ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, first_column_width);
    ImGui::TableSetupColumn("Values");
}

static void end_content_table() {
    ImGui::EndTable();
    ImGui::PopStyleVar();
}

void dnd::DisplayVisitor::visit(const Character* character_ptr) {
    begin_content_table(character_ptr);

    label("Type:");
    ImGui::Text("Character");
    source(character_ptr);
    label("Level:");
    ImGui::Text("%d", character_ptr->getLevel());
    label("XP:");
    ImGui::Text("%d", character_ptr->getXP());
    label("Stats:");
    ImGui::Text(
        "STR %d, DEX %d, CON %d, INT %d, WIS %d, CHA %d", character_ptr->state.attributes.at("STR") / 100,
        character_ptr->state.attributes.at("DEX") / 100, character_ptr->state.attributes.at("CON") / 100,
        character_ptr->state.attributes.at("INT") / 100, character_ptr->state.attributes.at("WIS") / 100,
        character_ptr->state.attributes.at("CHA") / 100
    ); // TODO - this is a hack, fix it

    label("Race:");
    if (ImGui::CollapsingHeader(character_ptr->race_ptr->name.c_str())) {
        visit(character_ptr->race_ptr);
    }

    if (character_ptr->subrace_ptr != nullptr) {
        label("Subrace:");
        if (ImGui::CollapsingHeader(character_ptr->subrace_ptr->name.c_str())) {
            visit(character_ptr->subrace_ptr);
        }
    }

    label("Class:");
    if (ImGui::CollapsingHeader(character_ptr->class_ptr->name.c_str())) {
        visit(character_ptr->class_ptr);
    }

    if (character_ptr->subclass_ptr != nullptr) {
        label("Subclass:");
        if (ImGui::CollapsingHeader(character_ptr->subclass_ptr->name.c_str())) {
            visit(character_ptr->subclass_ptr);
        }
    }

    end_content_table();
}

void dnd::DisplayVisitor::visit(const CharacterClass* character_class_ptr) {
    begin_content_table(character_class_ptr);

    label("Type:");
    ImGui::Text("Class");
    source(character_class_ptr);
    label("Hit Die:");
    ImGui::Text("%s", diceToString(character_class_ptr->hit_dice).c_str());
    label("ASI Levels:");
    std::stringstream asi_sstr;
    bool first = true;
    for (auto asi_level : character_class_ptr->asi_levels) {
        if (first) {
            first = false;
        } else {
            asi_sstr << ", ";
        }
        asi_sstr << asi_level;
    }
    ImGui::Text("%s", asi_sstr.str().c_str());
    label("Subclass Level:");
    ImGui::Text("%d", character_class_ptr->subclass_level);
    label("Features:");
    list_features(character_class_ptr);

    end_content_table();
}

void dnd::DisplayVisitor::visit(const CharacterSubclass* character_subclass_ptr) {
    begin_content_table(character_subclass_ptr);

    label("Type:");
    ImGui::Text("Subclass");
    source(character_subclass_ptr);
    label("Class name:");
    ImGui::Text("%s", character_subclass_ptr->class_name.c_str());
    label("Features:");
    list_features(character_subclass_ptr);

    end_content_table();
}

void dnd::DisplayVisitor::visit(const CharacterRace* character_race_ptr) {
    begin_content_table(character_race_ptr);

    label("Type:");
    ImGui::Text("Race");
    source(character_race_ptr);
    const char* has_subraces_cstr = character_race_ptr->has_subraces ? "yes" : "no";
    label("Has Subraces:");
    ImGui::Text("%s", has_subraces_cstr);
    label("Features:");
    list_features(character_race_ptr);

    end_content_table();
}

void dnd::DisplayVisitor::visit(const CharacterSubrace* character_subrace_ptr) {
    begin_content_table(character_subrace_ptr);

    label("Type:");
    ImGui::Text("Subrace");
    source(character_subrace_ptr);
    label("Race name:");
    ImGui::Text("%s", character_subrace_ptr->race_name.c_str());
    label("Features:");
    list_features(character_subrace_ptr);

    end_content_table();
}

void dnd::DisplayVisitor::visit(const Item* item_ptr) {
    begin_content_table(item_ptr);

    label("Type:");
    ImGui::Text("Item");
    source(item_ptr);
    label("Attunement");
    const char* attunement = item_ptr->requires_attunement ? "required" : "not required";
    ImGui::Text("%s", attunement);
    label("Description:");
    display_formatted_text(item_ptr->description);
    label("Cosmetic Description:");
    display_formatted_text(item_ptr->cosmetic_description);

    end_content_table();
}

void dnd::DisplayVisitor::visit(const Spell* spell_ptr) {
    begin_content_table(spell_ptr);

    label("Type:");
    ImGui::Text("Spell");
    source(spell_ptr);
    label("Spell Type:");
    ImGui::Text("%s", spell_ptr->type.str().c_str());
    label("Casting Time:");
    ImGui::Text("%s", spell_ptr->casting_time.c_str());
    label("Range:");
    ImGui::Text("%s", spell_ptr->range.c_str());
    label("Components:");
    ImGui::TextWrapped("%s", spell_ptr->components.str().c_str());
    label("Duration:");
    ImGui::Text("%s", spell_ptr->duration.c_str());

    label("Description:");
    display_formatted_text(spell_ptr->description);

    end_content_table();
}

void dnd::DisplayVisitor::visit(const Feature* feature_ptr) {
    begin_content_table(feature_ptr);

    label("Type:");
    ImGui::Text("Feature");
    source(feature_ptr);
    label("Description:");
    display_formatted_text(feature_ptr->description);

    end_content_table();
}

void dnd::DisplayVisitor::visit(const Choosable* choosable_ptr) {
    begin_content_table(choosable_ptr);

    label("Type:");
    ImGui::Text("Choosable");
    source(choosable_ptr);
    label("Description:");
    display_formatted_text(choosable_ptr->description);

    end_content_table();
}

void dnd::DisplayVisitor::display_formatted_text(const std::string& formatted_text) {
    std::vector<std::unique_ptr<Format>> text_formats = string_formatter.parse_formats(formatted_text);
    for (auto it = text_formats.begin(); it != text_formats.end(); ++it) {
        (*it)->accept(&display_format_visitor);
    }
}

void dnd::DisplayVisitor::list_features(const dnd::FeatureHolder* feature_holder_ptr) {
    if (feature_holder_ptr->features.empty()) {
        return;
    }
    for (const dnd::Feature& feature : feature_holder_ptr->features) {
        ImGui::Separator();
        if (ImGui::TreeNode(feature.name.c_str())) {
            ImGui::Separator();
            visit(&feature);
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
}
