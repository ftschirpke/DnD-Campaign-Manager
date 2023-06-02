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
#include "core/models/effect_holder/choosable.hpp"
#include "core/models/effect_holder/feature.hpp"
#include "core/models/item.hpp"
#include "core/models/spell.hpp"
#include "core/output/string_formatting/formats/format.hpp"
#include "core/output/string_formatting/string_formatter.hpp"

static const ImVec2 cell_padding = ImVec2(5, 5);
static const ImGuiTableFlags content_table_flags = ImGuiTableFlags_NoBordersInBodyUntilResize;
static const float first_column_width = 150;

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
    DND_UNUSED(item_ptr);
    // TODO
}

void dnd::DisplayVisitor::visit(const Spell* spell_ptr) {
    DND_UNUSED(spell_ptr);
    // TODO
}

void dnd::DisplayVisitor::visit(const Feature* feature_ptr) {
    std::string table_id = fmt::format("{}_table", feature_ptr->name);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
    if (ImGui::BeginTable(table_id.c_str(), 2, content_table_flags)) {
        ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, first_column_width);
        ImGui::TableSetupColumn("Values");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Type:");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Feature");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Source:");
        ImGui::TableSetColumnIndex(1);
        display_formatted_text(feature_ptr->source_file_path.string());

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Description:");
        ImGui::TableSetColumnIndex(1);
        display_formatted_text(feature_ptr->description);

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}

void dnd::DisplayVisitor::visit(const Choosable* choosable_ptr) {
    std::string table_id = fmt::format("{}_table", choosable_ptr->name);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
    if (ImGui::BeginTable(table_id.c_str(), 2, content_table_flags)) {
        ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, first_column_width);
        ImGui::TableSetupColumn("Values");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Type:");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Choosable");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Source:");
        ImGui::TableSetColumnIndex(1);
        display_formatted_text(choosable_ptr->source_file_path.string());

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Description:");
        ImGui::TableSetColumnIndex(1);
        display_formatted_text(choosable_ptr->description);

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}

void dnd::DisplayVisitor::display_formatted_text(const std::string& formatted_text) {
    std::vector<std::unique_ptr<Format>> text_formats = string_formatter.parse_formats(formatted_text);
    for (auto it = text_formats.begin(); it != text_formats.end(); ++it) {
        (*it)->accept(&display_format_visitor);
    }
}
