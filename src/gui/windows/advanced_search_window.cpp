#include <dnd_config.hpp>

#include "advanced_search_window.hpp"

#include <cassert>
#include <variant>

#include <imgui/imgui.h>

#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/session.hpp>
#include <gui/visitors/filters/filter_setting_visitor.hpp>

static constexpr ImVec2 cell_padding = ImVec2(5, 5);
static constexpr ImGuiTableFlags content_table_flags = ImGuiTableFlags_NoBordersInBodyUntilResize;
static constexpr float first_column_width = 250;
static constexpr float second_column_width = 250;
static constexpr float third_column_width = 250;
static constexpr float fourth_column_width = 250;
static constexpr float first_column_button_width = 162.5f;

static constexpr std::array<const char*, 10> content_filter_names = {
    "Any", "Characters", "Classes", "Subclasses", "Races", "Subraces", "Items", "Spells", "Features", "Choosables",
};

dnd::AdvancedSearchWindow::AdvancedSearchWindow(Session& session) : session(session) {}

void dnd::AdvancedSearchWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Advanced Search");

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
    bool table_exists = ImGui::BeginTable("advanced_search_filters", 5, content_table_flags);
    if (!table_exists) {
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }
    ImGui::TableSetupColumn("Filter", ImGuiTableColumnFlags_WidthFixed, first_column_width);
    ImGui::TableSetupColumn("Subfilter Name", ImGuiTableColumnFlags_WidthFixed, second_column_width);
    ImGui::TableSetupColumn("Subfilter Type", ImGuiTableColumnFlags_WidthFixed, third_column_width);
    ImGui::TableSetupColumn("Subfilter Value", ImGuiTableColumnFlags_WidthFixed, fourth_column_width);
    ImGui::TableSetupColumn("Other");
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    ContentFilterVariant& filter = session.get_advanced_search_filter();
    const size_t idx = filter.index();
    size_t swap_to_idx = content_filter_names.size();
    if (ImGui::BeginCombo("##Content Filter Type", content_filter_names[idx])) {
        for (size_t i = 0; i < content_filter_names.size(); ++i) {
            bool is_selected = idx == i;
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            } else if (ImGui::Selectable(content_filter_names[i], is_selected)) {
                swap_to_idx = i;
            }
        }
        ImGui::EndCombo();
    }
    if (swap_to_idx != content_filter_names.size()) {
        switch (swap_to_idx) {
            // TODO: Add all filters
            case 0:
                filter = ContentPieceFilter();
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                filter = SpellFilter();
                break;
            case 8:
                break;
            case 9:
                break;
            default:
                assert(false);
        }
    }

    std::visit(filter_setting_visitor, filter);
    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    if (ImGui::Button("Clear Filter", ImVec2(first_column_button_width, 0))) {
        std::visit([](ContentFilter& filter) { filter.clear(); }, filter);
    }

    ImGui::EndTable();
    ImGui::PopStyleVar();

    ImGui::Separator();
    ImGui::Spacing();
    if (ImGui::Button("Search", ImVec2(first_column_button_width, 0))) {
    }
    ImGui::Spacing();
    ImGui::Separator();

    ImGui::End();
}
