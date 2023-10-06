#include <dnd_config.hpp>

#include "content_window.hpp"
#include "gui/visitors/content/display_visitor.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

dnd::ContentWindow::ContentWindow(Session& session) : session(session), display_visitor() {}

void dnd::ContentWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Content");
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable;
    if (ImGui::BeginTabBar("Content Tabs", tab_bar_flags)) {
        std::deque<const ContentPiece*>& open_content_pieces = session.get_open_content_pieces();
        for (auto it = open_content_pieces.begin(); it != open_content_pieces.end();) {
            bool open = true;
            if (ImGui::BeginTabItem((*it)->get_name().c_str(), &open)) {
                ImGui::SeparatorText((*it)->get_name().c_str());
                (*it)->accept(display_visitor);
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
        const ContentPiece* selected_content_piece = session.get_selected_content_piece();
        if (selected_content_piece != nullptr) {
            ImGuiTabBar* tab_bar = ImGui::GetCurrentTabBar();
            ImGuiTabItem* tab_item = ImGui::TabBarFindTabByID(
                tab_bar, ImGui::GetID(selected_content_piece->get_name().c_str())
            );
            ImGui::TabBarQueueFocus(tab_bar, tab_item);
            selected_content_piece = nullptr;
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}
