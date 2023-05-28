#include "dnd_config.hpp"

#include "gui_app.hpp"

#include <filesystem>
#include <future>
#include <iostream>

#include <imgui/imfilebrowser.h>
#include <imgui/imgui.h>

#include "core/controllers/content_holder.hpp"
#include "core/parsing/controllers/content_parser.hpp"
#include "core/parsing/parsing_exceptions.hpp"

static const ImGuiFileBrowserFlags content_dir_dialog_options = ImGuiFileBrowserFlags_SelectDirectory;
static const ImGuiWindowFlags error_popup_options = ImGuiWindowFlags_AlwaysAutoResize;

dnd::GUIApp::GUIApp() : show_demo_window(false), is_parsing(false), content_dir_dialog(content_dir_dialog_options) {}

void dnd::GUIApp::initialize_gui_elements() {
    content_dir_dialog.SetTitle("Select content directory");
    content_dir_dialog.Open();
}

void dnd::GUIApp::render() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    render_content_dir_selection();
    render_campaign_selection();
    render_main_window();

    if (!content.empty()) {
        render_content_window();
    }
}

bool content_directory_valid(const std::filesystem::path& content_directory) {
    if (!std::filesystem::exists(content_directory)) {
        std::cout << "Content directory does not exist" << '\n';
        return false;
    }
    if (!std::filesystem::is_directory(content_directory)) {
        std::cout << "Content directory is not a directory" << '\n';
        return false;
    }
    if (!std::filesystem::exists(content_directory / "general")) {
        std::cout << "Content directory does not contain a general directory" << '\n';
        return false;
    }
    if (!std::filesystem::is_directory(content_directory / "general")) {
        std::cout << "Content directory does not contain a general directory (it is a file)" << '\n';
        return false;
    }
    return true;
}

void dnd::GUIApp::render_content_dir_selection() {
    content_dir_dialog.Display();
    if (content_dir_dialog.HasSelected()) {
        content_dir_dialog.Close();
        content_directory = content_dir_dialog.GetSelected();
        if (content_directory_valid(content_directory)) {
            ImGui::OpenPopup("Select campaign");
        } else {
            ImGui::OpenPopup("Invalid content directory");
        }
    }

    if (ImGui::BeginPopupModal("Invalid content directory", NULL, error_popup_options)) {
        ImGui::Text("Selected directory: %s", content_directory.string().c_str());
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            content_dir_dialog.Open();
        }
        ImGui::EndPopup();
    }
}

void dnd::GUIApp::render_campaign_selection() {
    if (ImGui::BeginPopupModal("Select campaign")) {
        for (const auto& entry : std::filesystem::directory_iterator(content_directory)) {
            if (!entry.is_directory() || entry.path().filename() == "general") {
                continue;
            }
            if (ImGui::Button(entry.path().filename().string().c_str())) {
                campaign_name = entry.path().filename().string();
                ImGui::CloseCurrentPopup();
                start_parsing();
            }
        }
        ImGui::Separator();
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            content_dir_dialog.Open();
        }
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void dnd::GUIApp::render_main_window() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Main Window - DnD Campaign Manager");

    if (!content_directory.empty()) {
        ImGui::Text("Content directory: %s", content_directory.string().c_str());
    }

    const char* content_dir_button_text = content_directory.empty() ? "Select content directory"
                                                                    : "Change content directory";

    if (ImGui::Button(content_dir_button_text)) {
        content_dir_dialog.Open();
    }

    if (!content_directory.empty()) {
        if (!campaign_name.empty()) {
            ImGui::Text("Campaign name: %s", campaign_name.c_str());
        }

        const char* campaign_button_text = campaign_name.empty() ? "Select campaign" : "Change campaign";

        if (ImGui::Button(campaign_button_text)) {
            ImGui::OpenPopup("Select campaign");
        }
    }

    if (is_parsing) {
        ImGui::Text("Parsing...");
        if (parsed_content.valid()) {
            is_parsing = false;
            try {
                content = parsed_content.get();
                content.finished_parsing();
                ImGui::Text("Parsing done");
            } catch (const parsing_error& e) {
                ImGui::OpenPopup("Parsing error");
            } catch (const std::exception& e) {
                std::cout << "Unknown error while parsing content: " << e.what() << '\n';
            }
        }
    }

    if (ImGui::BeginPopupModal("Parsing error", NULL, error_popup_options)) {
        ImGui::Text("Error while parsing content");
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            content_dir_dialog.Open();
        } else if (ImGui::Button("Select other campaign")) {
            ImGui::CloseCurrentPopup();
            ImGui::OpenPopup("Select campaign");
        } else if (ImGui::Button("Retry")) {
            ImGui::CloseCurrentPopup();
            start_parsing();
        } else if (ImGui::Button("Exit")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::Separator();
    ImGui::Checkbox("Show Demo Window", &show_demo_window);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

void dnd::GUIApp::start_parsing() {
    std::cout << "Would like to start Parsing content" << '\n';
    parsed_content = std::async(std::launch::async, &ContentParser::parse, &parser, content_directory, campaign_name);
    is_parsing = true;
}

void dnd::GUIApp::render_content_window() {
    ImGui::Begin("Content Window - DnD Campaign Manager");

    ImGui::Text("Content directory: %s", content_directory.string().c_str());
    ImGui::Text("Campaign name: %s", campaign_name.c_str());

    ImGui::Text("Status:\n%s", content.printStatus().c_str());

    ImGui::End();
}
