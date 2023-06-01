#include "dnd_config.hpp"

#include "gui_app.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>
#include <imgui/imfilebrowser.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <nlohmann/json.hpp>

#include "core/controllers/content_holder.hpp"
#include "core/controllers/searching/content_search.hpp"
#include "core/models/effect_holder/feature.hpp"
#include "core/models/item.hpp"
#include "core/models/spell.hpp"
#include "core/parsing/controllers/content_parser.hpp"
#include "core/parsing/parsing_exceptions.hpp"
#include "core/visitors/list_visitor.hpp"

static const char* const imgui_ini_filename = "imgui.ini";
static const char* const last_session_filename = "last_session.ini";

static const ImGuiFileBrowserFlags content_dir_dialog_options = ImGuiFileBrowserFlags_SelectDirectory;
static const ImGuiWindowFlags error_popup_options = ImGuiWindowFlags_AlwaysAutoResize;

dnd::GUIApp::GUIApp()
    : io(ImGui::GetIO()), show_demo_window(false), select_campaign(false), is_parsing(false), search(nullptr),
      search_result_count(0), content_dir_dialog(content_dir_dialog_options) {
    for (size_t i = 0; i < max_search_results; ++i) {
        old_selected_search_results[i] = false;
    }
}

void dnd::GUIApp::initialize() {
    content_dir_dialog.SetTitle("Select content directory");
    get_last_session_values();

    if (content_directory.empty()) {
        content_dir_dialog.Open();
    } else if (campaign_name.empty()) {
        select_campaign = true;
    } else {
        start_parsing();
    }
    search_query.reserve(100);
}

void dnd::GUIApp::clean_up() { save_session_values(); }

void dnd::GUIApp::get_last_session_values() {
    if (!std::filesystem::exists(last_session_filename)) {
        return;
    }
    std::ifstream last_session_file(last_session_filename);
    if (!last_session_file.is_open()) {
        return;
    }
    nlohmann::json last_session;
    last_session_file >> last_session;
    last_session_file.close();

    if (!last_session.contains("content_directory")) {
        return;
    }
    content_directory = std::filesystem::path(last_session["content_directory"].get<std::string>());
    if (last_session.contains("campaign_name")) {
        campaign_name = last_session["campaign_name"].get<std::string>();
    }
}

void dnd::GUIApp::save_session_values() {
    nlohmann::json last_session;
    if (!content_directory.empty()) {
        last_session["content_directory"] = content_directory.string();
        if (!campaign_name.empty()) {
            last_session["campaign_name"] = campaign_name;
        }
    }
    std::ofstream last_session_file(last_session_filename);
    last_session_file << std::setw(4) << last_session;
    last_session_file.close();
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
        render_search_window();
        render_status_window();
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
                search = std::make_unique<ContentSearch>(content);
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

void dnd::GUIApp::render_search_window() {
    ImGui::Begin("Main");

    if (ImGui::InputText("Search", &search_query, ImGuiInputTextFlags_EscapeClearsAll, nullptr, nullptr)) {
        search->set_search_query(search_query);
        if (search_query.size() > 1) {
            auto tolower = [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); };
            std::transform(search_query.begin(), search_query.end(), search_query.begin(), tolower);
            std::vector<const ContentPiece*> vec_search_results = search->get_sorted_results();
            search_result_count = vec_search_results.size();
            if (search_result_count <= 100) {
                for (size_t i = 0; i < search_result_count; ++i) {
                    search_results[i] = vec_search_results[i];
                    selected_search_results[i] = false;
                }
                ListVisitor list_visitor;
                for (size_t i = 0; i < search_result_count; ++i) {
                    search_results[i]->accept(&list_visitor);
                }
                std::vector<std::string> vec_list_strings = list_visitor.get_list();
                assert(vec_list_strings.size() == search_result_count);
                for (size_t i = 0; i < search_result_count; ++i) {
                    search_result_strings[i] = vec_list_strings[i];
                }
            }
        } else {
            search_result_count = 0;
        }
    }
    if (search_query.size() < 2) {
        ImGui::Text("Enter at least 2 characters to search.");
        ImGui::End();
        return;
    }
    if (search_result_count == 0) {
        ImGui::Text("No results. Please broaden your search.");
        ImGui::End();
        return;
    }
    if (search_result_count > 100) {
        ImGui::Text("Too many results. Please refine your search.");
        ImGui::End();
        return;
    }

    for (size_t i = 0; i < search_result_count; ++i) {
        if (ImGui::Selectable(search_result_strings[i].c_str(), false)) {
            selected_search_results[i] = true;
        }
    }
    ImGui::End();
}

void dnd::GUIApp::render_content_window() {
    ImGui::Begin("Content");
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable;
    if (ImGui::BeginTabBar("Content Tabs", tab_bar_flags)) {
        for (size_t i = 0; i < search_result_count; ++i) {
            if (selected_search_results[i]) {
                if (ImGui::BeginTabItem(search_results[i]->name.c_str(), &(selected_search_results[i]))) {
                    ImGui::Text("TESTING - %s", search_results[i]->name.c_str());
                    ImGui::Text("%s", search_result_strings[i].c_str());
                    ImGui::EndTabItem();
                }
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void dnd::GUIApp::render_spell_tab(const dnd::Spell* spell_ptr, size_t index) {
    if (ImGui::BeginTabItem(spell_ptr->name.c_str(), &old_selected_search_results[index])) {
        ImGui::Text("Spell - %s", spell_ptr->name.c_str());
        ImGui::Text("Type: %s", spell_ptr->type.str().c_str());
        ImGui::Text("Casting time: %s", spell_ptr->casting_time.c_str());
        ImGui::Text("Range: %s", spell_ptr->range.c_str());
        ImGui::TextWrapped("Components: %s", spell_ptr->components.str().c_str());
        ImGui::Text("Duration: %s", spell_ptr->duration.c_str());
        ImGui::TextWrapped("Description: %s", spell_ptr->description.c_str());
        ImGui::EndTabItem();
    }
}

void dnd::GUIApp::render_item_tab(const dnd::Item* item_ptr, size_t index) {
    if (ImGui::BeginTabItem(item_ptr->name.c_str(), &old_selected_search_results[index])) {
        ImGui::Text("Item - %s", item_ptr->name.c_str());
        ImGui::Text("Requires attunement: %s", item_ptr->requires_attunement ? "yes" : "no");
        ImGui::TextWrapped("Description: %s", item_ptr->description.c_str());
        ImGui::TextWrapped("Cosmetic Description: %s", item_ptr->cosmetic_description.c_str());
        ImGui::EndTabItem();
    }
}

void dnd::GUIApp::render_feature_tab(const dnd::Feature* feature_ptr, size_t index) {
    if (ImGui::BeginTabItem(feature_ptr->name.c_str(), &old_selected_search_results[index])) {
        ImGui::Text("Feature - %s", feature_ptr->name.c_str());
        ImGui::TextWrapped("Description: %s", feature_ptr->description.c_str());
        ImGui::EndTabItem();
    }
}

void dnd::GUIApp::render_status_window() {
    ImGui::Begin("Status");

    ImGui::Text("Content directory:\n%s", content_directory.string().c_str());
    ImGui::Text("Campaign name:\n%s", campaign_name.c_str());
    ImGui::Separator();

    ImGui::Text("Status:\n%s", content.printStatus().c_str());

    ImGui::End();
}
