#include <dnd_config.hpp>

#include "gui_app.hpp"

#include <algorithm>
#include <filesystem>
#include <string>

#include <imgui/imgui.h>

#include <core/content.hpp>
#include <core/session.hpp>
#include <core/utils/string_manipulation.hpp>
#include <gui/windows/content_configuration_window.hpp>
#include <gui/windows/content_window.hpp>
#include <gui/windows/error_messages_window.hpp>
#include <gui/windows/fuzzy_search_window.hpp>

namespace dnd {

static const char* const imgui_ini_filename = "imgui.ini";

static const ImGuiWindowFlags error_popup_options = ImGuiWindowFlags_AlwaysAutoResize;

GuiApp::GuiApp(const GuiFonts& fonts)
    : show_demo_window(false), show_advanced_search_window(false), show_pdf_create_window(false), session(),
      content_configuration_window(session), content_window(session, fonts), error_messages_window(session),
      fuzzy_search_window(session), advanced_search_window(session), pdf_create_window(session) {
    ImGui::GetIO().IniFilename = imgui_ini_filename;
}

void GuiApp::initialize() {
    session.retrieve_last_session_values();
    content_configuration_window.initialize();
}

void GuiApp::render() {
    DND_MEASURE_FUNCTION();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    render_overview_window();
    content_configuration_window.render();

    if (!session.get_unknown_error_messages().empty()) {
        render_parsing_error_popup();
    }
    error_messages_window.render();

    if (session.parsing_result_available()) {
        fuzzy_search_window.render();
        if (show_advanced_search_window) {
            advanced_search_window.render();
        }
        content_window.render();

        if (show_pdf_create_window) {
            pdf_create_window.render();
        }
    }
}

static const int r_offset = 25;
static void display_size(const char* const name, size_t s, float w) {
    ImGui::Text("%s", name);
    std::string count_string = std::to_string(s);
    float offset = w - r_offset - ImGui::CalcTextSize(count_string.c_str()).x;
    ImGui::SameLine(offset);
    ImGui::Text("%zu", s);
}

static const float min_w = 240.0f;
static void render_content_count_table(const Content& content) {
    float window_width = ImGui::GetWindowWidth();
    float w = std::max(min_w, window_width);
    display_size("Characters", content.get_characters().size(), w);
    display_size("Classes", content.get_classes().size(), w);
    display_size("Subclasses", content.get_subclasses().size(), w);
    display_size("Species", content.get_species().size(), w);
    display_size("Subspecies", content.get_subspecies().size(), w);
    display_size("Items", content.get_items().size(), w);
    display_size("Spells", content.get_spells().size(), w);
    display_size("Features", content.get_features().size(), w);
    display_size("Class Features", content.get_class_features().size(), w);
    display_size("Choosables", content.get_choosables().size(), w);
}

void GuiApp::render_overview_window() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Overview");

    ImGui::SeparatorText("Content selection");

    if (!session.get_content_directories().empty()) {
        ImGui::Text("Content directories:");
        for (const std::filesystem ::path& content_dir : session.get_content_directories()) {
            ImGui::TextWrapped("%s", content_dir.c_str());
        }
    }

    const char* content_dir_button_text = session.get_content_directories().empty() ? "Select content directory"
                                                                                    : "Add content directory";

    if (ImGui::Button(content_dir_button_text)) {
        content_configuration_window.open_content_directory_selection();
    }


    if (session.get_status() != SessionStatus::PARSING) {
        if (ImGui::Button("Parse content")) {
            // FIX: re-parsing does not work currently
            session.start_parsing();
        }
    }

#if DND_DEBUG_MODE
    ImGui::SeparatorText("Dev-Info");
    ImGui::Checkbox("Show Demo Window", &show_demo_window);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average\n%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
#endif

    ImGui::SeparatorText("Status");
    switch (session.get_status()) {
        case SessionStatus::CONTENT_DIR_SELECTION:
            ImGui::Text("Select one or more content directories and start parsing.");
            break;
        case SessionStatus::PARSING:
            ImGui::Text("Parsing...");
            break;
        case SessionStatus::READY:
            render_content_count_table(session.get_content());
            break;
        case SessionStatus::UNKNOWN_ERROR:
            ImGui::Text("An unknown error occurred");
            break;
    }

    ImGui::SeparatorText("Windows");

    if (session.parsing_result_available()) {
        ImGui::Checkbox("Advanced Search", &show_advanced_search_window);
        ImGui::Checkbox("PDF Creation", &show_pdf_create_window);
    }

    ImGui::End();
}

void GuiApp::render_parsing_error_popup() {
    DND_MEASURE_FUNCTION();
    ImGui::OpenPopup("Error");
    if (ImGui::BeginPopupModal("Error", nullptr, error_popup_options)) {
        for (const std::string& message : session.get_unknown_error_messages()) {
            ImGui::TextWrapped("%s", message.c_str());
            ImGui::Separator();
        }
        bool close = false;
        if (ImGui::Button("Select other directory")) {
            close = true;
            content_configuration_window.open_content_directory_selection();
        }
        ImGui::SameLine();
        if (ImGui::Button("Select other campaign")) {
            close = true;
            content_configuration_window.open_campaign_selection();
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            close = true;
        }
        if (close) {
            session.clear_unknown_error_messages();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

} // namespace dnd
