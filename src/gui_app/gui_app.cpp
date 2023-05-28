#include "dnd_config.hpp"

#include "gui_app.hpp"

#include <imgui/imgui.h>

dnd::GUIApp::GUIApp() : show_demo_window(false) {}

void dnd::GUIApp::render() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::Begin("Main Window - DnD Campaign Manager");

    ImGui::Text("Some sample text");
    ImGui::Checkbox("Show Demo Window", &show_demo_window);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
