#include <dnd_config.hpp>

#include "content_window.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <gui/gui_fonts.hpp>
#include <gui/visitors/content/display_visitor.hpp>

namespace dnd {

ContentWindow::ContentWindow(Session& session, const GuiFonts& fonts)
    : session(session), display_visitor(session.get_content(), fonts) {}

void ContentWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Content");
    std::deque<Id>& open_content_pieces = session.get_open_content_pieces();
    if (open_content_pieces.empty()) {
        ImGui::Text("No content selected");
        ImGui::End();
        return;
    }
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable;
    if (ImGui::BeginTabBar("Content Tabs", tab_bar_flags)) {
        for (auto it = open_content_pieces.begin(); it != open_content_pieces.end();) {
            Id content_piece_id = *it;
            auto content_piece = session.get_content().get(content_piece_id);
            const std::string& key = dispatch(content_piece, const auto& p, p.get().get_key());
            const std::string& name = dispatch(content_piece, const auto& p, p.get().get_name());
            bool open = true;
            if (ImGui::BeginTabItem(key.c_str(), &open)) {
                ImGui::SeparatorText(name.c_str());
                display_visitor.visit_variant(content_piece);
                ImGui::EndTabItem();
            }
            if (!open) {
                it = open_content_pieces.erase(it);
            } else {
                ++it;
            }
        }
        if (open_content_pieces.size()) {
            if (ImGui::TabItemButton(" X ", ImGuiTabItemFlags_Trailing)) {
                open_content_pieces.clear();
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Close all tabs");
            }
        }
        Opt<Id> selected_content_piece_id = session.get_selected_content_piece();
        if (selected_content_piece_id.has_value()) {
            auto content_piece = session.get_content().get(selected_content_piece_id.value());
            const std::string& key = dispatch(content_piece, const auto& p, p.get().get_key());
            ImGuiTabBar* tab_bar = ImGui::GetCurrentTabBar();
            ImGuiTabItem* tab_item = ImGui::TabBarFindTabByID(tab_bar, ImGui::GetID(key.c_str()));
            ImGui::TabBarQueueFocus(tab_bar, tab_item);
            selected_content_piece_id = std::nullopt;
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

} // namespace dnd
