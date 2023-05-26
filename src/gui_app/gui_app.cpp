#include "dnd_config.hpp"

#include "gui_app.hpp"

#include <imgui/imgui.h>

dnd::GUIApp::GUIApp() : show_demo_window(false), clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f)) {}

void dnd::GUIApp::render() {
    ImGuiIO& io = ImGui::GetIO();

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::Begin("Main Window - DnD Campaign Manager");

    ImGui::Text("Some sample text");
    ImGui::Checkbox("Show Demo Window", &show_demo_window);

    ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&clear_color));

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
