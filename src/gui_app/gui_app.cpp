#include <dnd_config.hpp>

#include "gui_app.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <imgui_filebrowser/imfilebrowser.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spell/spell.hpp>
#include <core/searching/content_search.hpp>
#include <core/session.hpp>
#include <core/utils/string_manipulation.hpp>
#include <gui_app/content_visitors/display_visitor.hpp>

static const char* const imgui_ini_filename = "imgui.ini";

static const ImGuiFileBrowserFlags content_dir_dialog_options = ImGuiFileBrowserFlags_SelectDirectory;
static const ImGuiWindowFlags error_popup_options = ImGuiWindowFlags_AlwaysAutoResize;

dnd::GUIApp::GUIApp()
    : show_demo_window(false), select_campaign(false), session(), content_dir_dialog(content_dir_dialog_options),
      last_session_open_tabs(), forced_next_selection(nullptr), display_visitor() {
    ImGui::GetIO().IniFilename = imgui_ini_filename;
}

void dnd::GUIApp::initialize() {
    content_dir_dialog.SetTitle("Select content directory");
    content_dir_dialog.SetWindowSize(1200, 900);
    session.retrieve_last_session_values();
    open_content_dir_dialog();

    switch (session.get_status()) {
        case SessionStatus::CONTENT_DIR_SELECTION:
            content_dir_dialog.Open();
            break;
        case SessionStatus::CAMPAIGN_SELECTION:
            select_campaign = true;
            break;
        default:
            break;
    }
}

void dnd::GUIApp::render() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    session.update();

    render_content_dir_selection();
    render_campaign_selection();
    render_overview_window();
    if (!session.get_error_messages().empty()) {
        render_parsing_error_popup();
    }

    if (!session.get_content().empty()) {
        render_search_window();
        render_content_window();
    }
}

void dnd::GUIApp::open_content_dir_dialog() {
    content_dir_dialog.SetPwd(session.get_content_directory().parent_path());
    content_dir_dialog.Open();
}

void dnd::GUIApp::render_content_dir_selection() {
    content_dir_dialog.Display();
    if (content_dir_dialog.HasSelected()) {
        content_dir_dialog.Close();
        bool valid = session.set_content_directory(content_dir_dialog.GetSelected());
        if (valid) {
            select_campaign = session.get_status() == SessionStatus::CAMPAIGN_SELECTION;
        } else {
            ImGui::OpenPopup("Invalid content directory");
        }
    }

    if (ImGui::BeginPopupModal("Invalid content directory", nullptr, error_popup_options)) {
        ImGui::Text("Selected directory: %s", content_dir_dialog.GetSelected().string().c_str());
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            open_content_dir_dialog();
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
        for (const std::string& possible_campaign_name : session.get_possible_campaign_names()) {
            if (ImGui::Button(possible_campaign_name.c_str())) {
                close = session.set_campaign_name(possible_campaign_name);
            }
        }
        ImGui::Separator();
        if (ImGui::Button("Select other directory")) {
            ImGui::CloseCurrentPopup();
            close = true;
            open_content_dir_dialog();
        }
        if (close || ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            select_campaign = false;
        }
        ImGui::EndPopup();
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
static void render_content_count_table(const dnd::Content& content) {
    float window_width = ImGui::GetWindowWidth();
    float w = std::max(min_w, window_width);
    display_size("Characters", content.get_characters().size(), w);
    display_size("Classes", content.get_character_classes().size(), w);
    display_size("Subclasses", content.get_character_subclasses().size(), w);
    display_size("Races", content.get_character_races().size(), w);
    display_size("Subraces", content.get_character_subraces().size(), w);
    display_size("Items", content.get_items().size(), w);
    display_size("Spells", content.get_spells().size(), w);
    display_size("Features", content.get_features().size(), w);
    display_size("Choosables", content.get_choosable_features().size(), w);
}

void dnd::GUIApp::render_overview_window() {
    ImGui::Begin("Overview");

    ImGui::SeparatorText("Content selection");

    if (!session.get_content_directory().empty()) {
        ImGui::Text("Content directory:");
        ImGui::TextWrapped("%s", session.get_content_directory().string().c_str());
    }

    const char* content_dir_button_text = session.get_content_directory().empty() ? "Select content directory"
                                                                                  : "Change content directory";

    if (ImGui::Button(content_dir_button_text)) {
        open_content_dir_dialog();
    }

    if (!session.get_content_directory().empty()) {
        if (!session.get_campaign_name().empty()) {
            ImGui::Text("Campaign name: %s", session.get_campaign_name().c_str());
        }

        const char* campaign_button_text = session.get_campaign_name().empty() ? "Select campaign" : "Change campaign";

        if (ImGui::Button(campaign_button_text)) {
            select_campaign = true;
        }
    }

    ImGui::SeparatorText("Dev-Info");
    ImGui::Checkbox("Show Demo Window", &show_demo_window);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average\n%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::SeparatorText("Status");
    switch (session.get_status()) {
        case SessionStatus::CONTENT_DIR_SELECTION:
            ImGui::Text("You need to select a content directory");
            break;
        case SessionStatus::CAMPAIGN_SELECTION:
            ImGui::Text("You need to select a campaign");
            break;
        case SessionStatus::PARSING:
            ImGui::Text("Parsing...");
            break;
        case SessionStatus::READY:
            render_content_count_table(session.get_content());
            break;
        default:
            break;
    }

    ImGui::SeparatorText("Errors");
    if (ImGui::BeginChild("Error list")) {
        for (const std::string& message : session.get_error_messages()) {
            ImGui::TextWrapped("%s", message.c_str());
        }
        ImGui::SeparatorText("Parsing errors");
        for (const ParsingError& error : session.get_errors().get_parsing_errors()) {
            SourceInfo source_info(error.get_filepath());
            ImGui::TextWrapped(
                "%s (%s - %s - %s)", error.get_error_message().c_str(), source_info.get_source_group_name().c_str(),
                source_info.get_source_type_name().c_str(), source_info.get_source_name().c_str()
            );
        }
        ImGui::SeparatorText("Validation errors");
        for (const ValidationError& error : session.get_errors().get_validation_errors()) {
            if (error.get_validation_data() == nullptr) {
                ImGui::TextWrapped("%s", error.get_error_message().c_str());
            } else {
                SourceInfo source_info(error.get_validation_data()->source_path);
                ImGui::TextWrapped(
                    "%s (%s - %s - %s)", error.get_error_message().c_str(), source_info.get_source_group_name().c_str(),
                    source_info.get_source_type_name().c_str(), source_info.get_source_name().c_str()
                );
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

void dnd::GUIApp::render_parsing_error_popup() {
    ImGui::OpenPopup("Error");
    if (ImGui::BeginPopupModal("Error", nullptr, error_popup_options)) {
        for (const std::string& message : session.get_error_messages()) {
            ImGui::TextWrapped("%s", message.c_str());
            ImGui::Separator();
        }
        bool close = false;
        if (ImGui::Button("Select other directory")) {
            close = true;
            open_content_dir_dialog();
        }
        ImGui::SameLine();
        if (ImGui::Button("Select other campaign")) {
            close = true;
            select_campaign = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Retry")) {
            close = true;
            // TODO: retry parsing, if that's a wanted feature
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            close = true;
        }
        if (close) {
            session.clear_error_messages();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void dnd::GUIApp::render_search_window() {
    ImGui::Begin("Search");

    if (ImGui::InputText("Search", &search_query, ImGuiInputTextFlags_EscapeClearsAll, nullptr, nullptr)) {
        session.set_search_query(search_query);
    }
    ImGui::Separator();
    if (search_query.size() < 2) {
        ImGui::Text("Enter at least 2 characters to search.");
        ImGui::End();
        return;
    }
    size_t search_result_count = session.get_search_result_count();
    if (search_result_count == 0) {
        ImGui::Text("No results. Please broaden your search.");
        ImGui::End();
        return;
    }
    if (search_result_count > 100) {
        ImGui::Text("Too many results (%ld). Please refine your search.", search_result_count);
        ImGui::End();
        return;
    }

    if (ImGui::BeginChild("Search Results", ImVec2(-FLT_MIN, -FLT_MIN))) {
        std::vector<std::string> search_result_strings = session.get_search_result_strings();
        for (size_t i = 0; i < search_result_count; ++i) {
            if (ImGui::Selectable(search_result_strings[i].c_str(), false)) {
                session.open_search_result(i);
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

void dnd::GUIApp::render_content_window() {
    ImGui::Begin("Content");
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable;
    if (ImGui::BeginTabBar("Content Tabs", tab_bar_flags)) {
        std::deque<const ContentPiece*>& open_content_pieces = session.get_open_content_pieces();
        for (auto it = open_content_pieces.begin(); it != open_content_pieces.end();) {
            bool open = true;
            if (ImGui::BeginTabItem((*it)->get_name().c_str(), &open)) {
                ImGui::SeparatorText((*it)->get_name().c_str());
                (*it)->accept(&display_visitor);
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
        if (forced_next_selection != nullptr) {
            ImGuiTabBar* tab_bar = ImGui::GetCurrentTabBar();
            ImGuiTabItem* tab_item = ImGui::TabBarFindTabByID(
                tab_bar, ImGui::GetID(forced_next_selection->get_name().c_str())
            );
            ImGui::TabBarQueueFocus(tab_bar, tab_item);
            forced_next_selection = nullptr;
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}
