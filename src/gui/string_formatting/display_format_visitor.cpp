#include <dnd_config.hpp>

#include "display_format_visitor.hpp"

#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <imgui/imgui.h>

#include <core/output/string_formatting/format_visitor.hpp>
#include <core/output/string_formatting/formats/bulleted_list.hpp>
#include <core/output/string_formatting/formats/paragraph.hpp>
#include <core/output/string_formatting/formats/table.hpp>

dnd::DisplayFormatVisitor::DisplayFormatVisitor(ImGuiTableFlags table_flags) : table_flags(table_flags) {}

void dnd::DisplayFormatVisitor::visit(const BulletedList& bulleted_list) const {
    for (const auto& element : bulleted_list.get_items()) {
        ImGui::Bullet();
        ImGui::TextWrapped("%s", std::string(element).c_str());
    }
}

void dnd::DisplayFormatVisitor::visit(const Paragraph& paragraph) const {
    std::string_view text = paragraph.get_text();
    ImGui::TextWrapped("%s", std::string(text).c_str());
    if (paragraph.get_empty_line_after()) {
        ImGui::Spacing();
    }
}

void dnd::DisplayFormatVisitor::visit(const Table& table) const {
    std::vector<std::vector<std::string_view>> rows = table.get_rows();
    if (rows.empty() || rows[0].empty()) {
        return;
    }
    std::string table_id = fmt::format(
        "r{}_c{}_{}_{}", rows.size(), table.get_num_columns(), rows[0][0], rows.back().back()
    );
    if (ImGui::BeginTable(table_id.c_str(), static_cast<int>(table.get_num_columns()), table_flags)) {
        bool is_first = true;
        for (const auto& row : rows) {
            if (!is_first) {
                ImGui::TableNextRow();
            }
            for (const std::string_view& cell : row) {
                if (is_first) {
                    ImGui::TableSetupColumn(std::string(cell).c_str());
                } else {
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", std::string(cell).c_str());
                }
            }
            if (is_first) {
                ImGui::TableHeadersRow();
                is_first = false;
            }
        }
        ImGui::EndTable();
    }
}
