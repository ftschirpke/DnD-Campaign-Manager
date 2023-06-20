#include <dnd_config.hpp>

#include "gui_app.hpp"

#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>
#include <imgui/imfilebrowser.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <nlohmann/json.hpp>

#include <core/controllers/content_holder.hpp>
#include <core/controllers/searching/content_search.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spell/spell.hpp>
#include <core/parsing/content_parser.hpp>
#include <core/utils/string_manipulation.hpp>
#include <gui_app/content_visitors/display_visitor.hpp>
#include <gui_app/content_visitors/list_visitor.hpp>
#include <gui_app/content_visitors/session_visitor.hpp>

static const char* const imgui_ini_filename = "imgui.ini";
static const char* const last_session_filename = "last_session.ini";

static const ImGuiFileBrowserFlags content_dir_dialog_options = ImGuiFileBrowserFlags_SelectDirectory;
static const ImGuiWindowFlags error_popup_options = ImGuiWindowFlags_AlwaysAutoResize;

dnd::GUIApp::GUIApp()
    : show_demo_window(false), select_campaign(false), is_parsing(false),
      content_dir_dialog(content_dir_dialog_options), search(nullptr), search_result_count(0), display_visitor() {
    ImGui::GetIO().IniFilename = imgui_ini_filename;
}

dnd::GUIApp::~GUIApp() { clean_up(); }

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
    content_directory = last_session["content_directory"].get<std::filesystem::path>();
    if (last_session.contains("campaign_name")) {
        campaign_name = last_session["campaign_name"].get<std::string>();
    }

    if (!last_session.contains("open_tabs")) {
        return;
    }
    if (!last_session["open_tabs"].is_object()) {
        return;
    }
    last_session_open_tabs = last_session["open_tabs"].get<std::unordered_map<std::string, std::vector<std::string>>>();
}

void dnd::GUIApp::save_session_values() {
    nlohmann::json last_session;
    if (!content_directory.empty()) {
        last_session["content_directory"] = content_directory.string();
        if (!campaign_name.empty()) {
            last_session["campaign_name"] = campaign_name;
        }
    }

    SessionVisitor session_visitor;
    for (const auto open_content_piece : open_content_pieces) {
        open_content_piece->accept(&session_visitor);
    }
    last_session["open_tabs"] = session_visitor.get_open_tabs();

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
        render_content_window();
    }
}

static bool content_directory_valid(const std::filesystem::path& content_directory) {
    if (!std::filesystem::exists(content_directory)) {
        std::cerr << "Content directory does not exist" << '\n';
        return false;
    }
    if (!std::filesystem::is_directory(content_directory)) {
        std::cerr << "Content directory is not a directory" << '\n';
        return false;
    }
    if (!std::filesystem::exists(content_directory / "general")) {
        std::cerr << "Content directory does not contain a general directory" << '\n';
        return false;
    }
    if (!std::filesystem::is_directory(content_directory / "general")) {
        std::cerr << "Content directory does not contain a general directory (it is a file)" << '\n';
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

static const int r_offset = 25;
static void display_size(const char* const name, size_t s, float w) {
    ImGui::Text("%s", name);
    std::string count_string = std::to_string(s);
    float offset = w - r_offset - ImGui::CalcTextSize(count_string.c_str()).x;
    ImGui::SameLine(offset);
    ImGui::Text("%zu", s);
}

static const float min_w = 240.0f;
static void render_content_count_table(const dnd::ContentHolder& content) {
    float window_width = ImGui::GetWindowWidth();
    float w = std::max(min_w, window_width);
    display_size("Characters", content.characters.size(), w);
    display_size("Classes", content.character_classes.size(), w);
    display_size("Subclasses", content.character_subclasses.size(), w);
    display_size("Races", content.character_races.size(), w);
    display_size("Subraces", content.character_subraces.size(), w);
    display_size("Items", content.items.size(), w);
    display_size("Spells", content.spells.size(), w);
    display_size("Features", content.features.size(), w);
    display_size("Choosables", content.choosable_features.size(), w);
}

void dnd::GUIApp::render_overview_window() {
    ImGui::Begin("Overview");

    ImGui::SeparatorText("Content selection");

    if (!content_directory.empty()) {
        ImGui::Text("Content directory:");
        ImGui::TextWrapped("%s", content_directory.string().c_str());
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
        if (parsing_results.valid()) {
            finish_parsing();
        }
    }

    ImGui::SeparatorText("Dev-Info");
    ImGui::Checkbox("Show Demo Window", &show_demo_window);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average\n%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    ImGui::SeparatorText("Status");
    if (content.empty()) {
        ImGui::Text("No content loaded");
    } else {
        render_content_count_table(content);
    }

    ImGui::SeparatorText("Errors");
    if (ImGui::BeginChild("Error list")) {
        for (const std::string& message : error_messages) {
            ImGui::TextWrapped("%s", message.c_str());
        }
        ImGui::SeparatorText("Parsing errors");
        for (const ParsingError& error : errors.get_parsing_errors()) {
            SourceInfo source_info(error.get_filepath());
            ImGui::TextWrapped(
                "%s (%s - %s - %s)", error.get_error_message().c_str(), source_info.get_source_group_name().c_str(),
                source_info.get_source_type_name().c_str(), source_info.get_source_name().c_str()
            );
        }
        for (const ValidationError& error : errors.get_validation_errors()) {
            ImGui::TextWrapped("%s", error.get_error_message().c_str());
        }
        ImGui::EndChild();
    }


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
    open_content_pieces.clear();
    parsing_results = std::async(std::launch::async, &ContentParser::parse, &parser, content_directory, campaign_name);
    is_parsing = true;
}

void dnd::GUIApp::finish_parsing() {
    try {
        ParsingResult parsing_result = parsing_results.get();
        content = std::move(parsing_result.content);
        errors = std::move(parsing_result.errors);
        search = std::make_unique<ContentSearch>(content);
    } catch (const std::exception& e) {
        error_messages.push_back(e.what());
    }

    open_last_session_tabs();

    is_parsing = false;
}

void dnd::GUIApp::open_last_session_tabs() {
    for (const std::string& character_to_open : last_session_open_tabs["character"]) {
        open_content_pieces.push_back(&content.characters.get(character_to_open));
    }
    for (const std::string& character_class_to_open : last_session_open_tabs["character_class"]) {
        open_content_pieces.push_back(&content.character_classes.get(character_class_to_open));
    }
    for (const std::string& character_subclass_to_open : last_session_open_tabs["character_subclass"]) {
        open_content_pieces.push_back(&content.character_subclasses.get(character_subclass_to_open));
    }
    for (const std::string& character_race_to_open : last_session_open_tabs["character_race"]) {
        open_content_pieces.push_back(&content.character_races.get(character_race_to_open));
    }
    for (const std::string& character_subrace_to_open : last_session_open_tabs["character_subrace"]) {
        open_content_pieces.push_back(&content.character_subraces.get(character_subrace_to_open));
    }
    for (const std::string& item_to_open : last_session_open_tabs["item"]) {
        open_content_pieces.push_back(&content.items.get(item_to_open));
    }
    for (const std::string& spell_to_open : last_session_open_tabs["spell"]) {
        open_content_pieces.push_back(&content.spells.get(spell_to_open));
    }
    for (const std::string& feature_to_open : last_session_open_tabs["feature"]) {
        open_content_pieces.push_back(content.features.get(feature_to_open));
    }
    for (const std::string& choosable_to_open : last_session_open_tabs["choosable_feature"]) {
        open_content_pieces.push_back(content.choosable_features.get(choosable_to_open));
    }
}

void dnd::GUIApp::render_search_window() {
    ImGui::Begin("Search");

    if (ImGui::InputText("Search", &search_query, ImGuiInputTextFlags_EscapeClearsAll, nullptr, nullptr)) {
        search->set_search_query(search_query);
        if (search_query.size() > 1) {
            string_to_lowercase(search_query);
            std::vector<const ContentPiece*> vec_search_results = search->get_sorted_results();
            search_result_count = vec_search_results.size();
            if (search_result_count <= 100) {
                for (size_t i = 0; i < search_result_count; ++i) {
                    search_results[i] = vec_search_results[i];
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
    ImGui::Separator();
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

    if (ImGui::BeginChild("Search Results", ImVec2(-FLT_MIN, -FLT_MIN))) {
        for (size_t i = 0; i < search_result_count; ++i) {
            if (ImGui::Selectable(search_result_strings[i].c_str(), false)) {
                open_content_pieces.push_back(search_results[i]);
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void dnd::GUIApp::render_content_window() {
    ImGui::Begin("Content");
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable;
    if (ImGui::BeginTabBar("Content Tabs", tab_bar_flags)) {
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
        ImGui::EndTabBar();
    }
    ImGui::End();
}
