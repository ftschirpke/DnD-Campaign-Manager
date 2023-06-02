#include "dnd_config.hpp"

#include "display_format_visitor.hpp"

#include <string_view>

#include <fmt/format.h>
#include <imgui/imgui.h>

#include "core/output/string_formatting/format_visitor.hpp"
#include "core/output/string_formatting/formats/bulleted_list.hpp"
#include "core/output/string_formatting/formats/paragraph.hpp"
#include "core/output/string_formatting/formats/table.hpp"

void dnd::DisplayFormatVisitor::visit(BulletedList* bulleted_list) {
    for (auto& element : bulleted_list->get_elements()) {
        ImGui::BulletText("%s", element.c_str());
    }
}

void dnd::DisplayFormatVisitor::visit(Paragraph* paragraph) {
    std::string_view text = paragraph->get_text();
    if (text.empty()) {
        return;
    }
    ImGui::Text("%s", text.c_str());
    if (paragraph->get_empty_line_after()) {
        ImGui::Spacing();
    }
}

void dnd::DisplayFormatVisitor::visit(Table* table) {
    std::vector<std::vector<std::string_view>> rows = table->get_rows();
    if (rows.empty() || rows[0].empty()) {
        return;
    }
    std::string table_id = fmt::format(
        "r{}_c{}_{}_{}", rows.size(), table->get_num_columns(), rows[0][0], rows.back().back()
    );
    if (ImGui::BeginTable(table_id.c_str(), table->get_column_count())) {
        for (const auto& row : row) {
            ImGui::TableNextRow();
            for (const std::string_view& cell : row) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", cell.c_str());
            }
        }
        ImGui::EndTable();
    }
}
