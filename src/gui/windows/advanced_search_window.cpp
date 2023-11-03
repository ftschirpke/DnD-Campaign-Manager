#include <dnd_config.hpp>

#include "advanced_search_window.hpp"

#include <memory>
#include <vector>

#include <imgui/imgui.h>

#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/session.hpp>
#include <gui/visitors/filters/filter_setting_visitor.hpp>

static const ImVec2 cell_padding = ImVec2(5, 5);
static constexpr ImGuiTableFlags content_table_flags = ImGuiTableFlags_NoBordersInBodyUntilResize;
static const float first_column_width = 250;
static const float second_column_width = 250;
static const float third_column_width = 250;

dnd::AdvancedSearchWindow::AdvancedSearchWindow(Session& session) : session(session) {}

void dnd::AdvancedSearchWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Advanced Search");

    ImGui::Text("Advanced Search is gonne be here soon!");

    std::vector<ContentFilter*> filters = session.get_advanced_search_filters();

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
    ImGui::TableSetupColumn("Subfilter Value", ImGuiTableColumnFlags_WidthFixed, third_column_width);
    ImGui::TableSetupColumn("Other");
    ImGui::TableNextRow();

    for (ContentFilter* filter : filters) {
        filter->accept(filter_setting_visitor);
        ImGui::TableNextRow();
    }

    ImGui::TableSetColumnIndex(0);
    if (ImGui::Button("Add Content Filter")) {
        std::unique_ptr<ContentFilter> filter = std::make_unique<ContentPieceFilter>();
        session.add_advanced_search_filter(std::move(filter));
    }

    ImGui::EndTable();
    ImGui::PopStyleVar();

    if (ImGui::Button("Search")) {
    }

    ImGui::End();
}
