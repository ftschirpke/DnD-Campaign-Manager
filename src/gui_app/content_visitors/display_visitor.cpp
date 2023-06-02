#include "dnd_config.hpp"

#include "display_visitor.hpp"

#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <imgui/imgui.h>

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
    std::string table_id = fmt::format("{}_table", content_piece_ptr->name);
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
    DND_UNUSED(character_ptr);
    // TODO
}

void dnd::DisplayVisitor::visit(const CharacterClass* character_class_ptr) {
    DND_UNUSED(character_class_ptr);
    // TODO
}

void dnd::DisplayVisitor::visit(const CharacterSubclass* character_subclass_ptr) {
    DND_UNUSED(character_subclass_ptr);
    // TODO
}

void dnd::DisplayVisitor::visit(const CharacterRace* character_race_ptr) {
    DND_UNUSED(character_race_ptr);
    // TODO
}

void dnd::DisplayVisitor::visit(const CharacterSubrace* character_subrace_ptr) {
    DND_UNUSED(character_subrace_ptr);
    // TODO
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
