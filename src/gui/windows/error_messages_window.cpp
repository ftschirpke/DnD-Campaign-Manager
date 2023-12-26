#include <dnd_config.hpp>

#include "error_messages_window.hpp"

#include <imgui/imgui.h>

#include <core/session.hpp>

namespace dnd {

ErrorMessagesWindow::ErrorMessagesWindow(Session& session) : session(session) {}

void ErrorMessagesWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Errors");
    for (const std::string& message : session.get_unknown_error_messages()) {
        ImGui::TextWrapped("%s", message.c_str());
    }
    ImGui::SeparatorText("Parsing errors");
    for (const std::string& message : session.get_parsing_error_messages()) {
        ImGui::TextWrapped("%s", message.c_str());
    }
    ImGui::SeparatorText("Validation errors");
    for (const std::string& message : session.get_validation_error_messages()) {
        ImGui::TextWrapped("%s", message.c_str());
    }
    ImGui::End();
}

} // namespace dnd
