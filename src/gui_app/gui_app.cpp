#include "dnd_config.hpp"

#include "gui_app.hpp"

#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>
#include <imgui/imfilebrowser.h>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "core/controllers/content_holder.hpp"
#include "core/parsing/controllers/content_parser.hpp"
#include "core/parsing/parsing_exceptions.hpp"

static const ImGuiFileBrowserFlags content_dir_dialog_options = ImGuiFileBrowserFlags_SelectDirectory;
static const ImGuiWindowFlags error_popup_options = ImGuiWindowFlags_AlwaysAutoResize;

dnd::GUIApp::GUIApp()
    : show_demo_window(false), select_campaign(false), is_parsing(false),
      content_dir_dialog(content_dir_dialog_options) {}

void dnd::GUIApp::initialize() {
    content_dir_dialog.SetTitle("Select content directory");
    content_dir_dialog.Open();

    search_query.reserve(100);
}

void dnd::GUIApp::render() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    render_content_dir_selection();
    render_campaign_selection();
    render_overview_window();
    if (!error_messages.empty()) {
        render_parsing_error_popup();
    }

    if (!content.empty()) {
        render_content_window();
        render_status_window();
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
            select_campaign = true;
        } else {
            ImGui::OpenPopup("Invalid content directory");
        }
    }

    if (ImGui::BeginPopupModal("Invalid content directory", nullptr, error_popup_options)) {
        ImGui::Text("Selected directory: %s", content_directory.string().c_str());
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            content_dir_dialog.Open();
        }
        ImGui::EndPopup();
    }
}

void dnd::GUIApp::render_campaign_selection() {
    if (select_campaign) {
        ImGui::OpenPopup("Select campaign");
    }
    if (ImGui::BeginPopupModal("Select campaign")) {
        bool close = false;
        for (const auto& entry : std::filesystem::directory_iterator(content_directory)) {
            if (!entry.is_directory() || entry.path().filename() == "general") {
                continue;
            }
            if (ImGui::Button(entry.path().filename().string().c_str())) {
                campaign_name = entry.path().filename().string();
                close = true;
                start_parsing();
                break;
            }
        }
        ImGui::Separator();
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            close = true;
            content_dir_dialog.Open();
        }
        if (close || ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            select_campaign = false;
        }
        ImGui::EndPopup();
    }
}

void dnd::GUIApp::render_overview_window() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Overview");

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
            select_campaign = true;
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
                error_messages.push_back(e.what());
            } catch (const std::exception& e) {
                error_messages.push_back(e.what());
            }
        }
    }

    ImGui::Separator();
    ImGui::Checkbox("Show Demo Window", &show_demo_window);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}

void dnd::GUIApp::render_parsing_error_popup() {
    ImGui::OpenPopup("Error");
    if (ImGui::BeginPopupModal("Error", nullptr, error_popup_options)) {
        for (const std::string& message : error_messages) {
            ImGui::TextWrapped("%s", message.c_str());
            ImGui::Separator();
        }
        bool close = false;
        if (ImGui::Button("Select other directory")) {
            close = true;
            content_dir_dialog.Open();
        }
        ImGui::SameLine();
        if (ImGui::Button("Select other campaign")) {
            close = true;
            select_campaign = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Retry")) {
            close = true;
            start_parsing();
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            close = true;
        }
        if (close) {
            error_messages.clear();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void dnd::GUIApp::start_parsing() {
    parsed_content = std::async(std::launch::async, &ContentParser::parse, &parser, content_directory, campaign_name);
    is_parsing = true;
}

void dnd::GUIApp::render_content_window() {
    ImGui::Begin("Main");

    if (ImGui::InputText("Search", &search_query, ImGuiInputTextFlags_EscapeClearsAll, nullptr, nullptr)) {
        std::unordered_set<const dnd::Spell*> matched_spells = content.spells.prefix_get(search_query);
        for (const auto spell_ptr : matched_spells) {
            ImGui::Text("%s", spell_ptr->name.c_str());
        }
        std::unordered_set<const dnd::Item*> matched_items = content.items.prefix_get(search_query);
        for (const auto item_ptr : matched_items) {
            ImGui::Text("%s", item_ptr->name.c_str());
        }
        std::unordered_set<const dnd::Feature**> matched_features = content.features.prefix_get(search_query);
        for (const auto feature_ptr : matched_features) {
            ImGui::Text("%s", (*feature_ptr)->name.c_str());
        }
    }

    ImGui::End();
}

void dnd::GUIApp::on_search_input(ImGuiInputTextCallbackData* data) { std::cout << data->EventChar << '\n'; }

void dnd::GUIApp::render_status_window() {
    ImGui::Begin("Status");

    ImGui::Text("Content directory:\n%s", content_directory.string().c_str());
    ImGui::Text("Campaign name:\n%s", campaign_name.c_str());
    ImGui::Separator();

    ImGui::Text("Status:\n%s", content.printStatus().c_str());

    ImGui::End();
}
