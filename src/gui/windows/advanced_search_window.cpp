#include <dnd_config.hpp>

#include "advanced_search_window.hpp"

#include <imgui/imgui.h>

#include <core/session.hpp>

dnd::AdvancedSearchWindow::AdvancedSearchWindow(Session& session) : session(session) {}

void dnd::AdvancedSearchWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Advanced Search");

    ImGui::Text("Advanced Search is gonne be here soon!");

    ImGui::End();
}
