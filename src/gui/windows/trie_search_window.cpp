#include <dnd_config.hpp>

#include "trie_search_window.hpp"

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <array>
#include <string>

dnd::TrieSearchWindow::TrieSearchWindow(Session& session) : session(session), search_query(), search_options() {
    search_options.fill(true);
}

static void trie_search_option_line(size_t index, const char* const name, std::array<bool, 9>& options) {
    std::string only_button_label = fmt::format("Only##{}", index);
    if (ImGui::Button(only_button_label.c_str())) {
        options.fill(false);
        options[index] = true;
    }
    ImGui::SameLine();
    ImGui::Checkbox(name, &options[index]);
}

void dnd::TrieSearchWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Simple Search");

    bool search_changed = false;
    if (ImGui::InputText("Search", &search_query, ImGuiInputTextFlags_EscapeClearsAll, nullptr, nullptr)) {
        search_changed = true;
    }
    const std::array<bool, 9> old_trie_search_options = search_options;
    if (ImGui::TreeNode("Search options")) {
        if (ImGui::Button("All", ImVec2(200, 0))) {
            search_options.fill(true);
        }
        trie_search_option_line(0, "Characters", search_options);
        trie_search_option_line(1, "Races", search_options);
        trie_search_option_line(3, "Subraces", search_options);
        trie_search_option_line(2, "Classes", search_options);
        trie_search_option_line(4, "Subclasses", search_options);
        trie_search_option_line(5, "Items", search_options);
        trie_search_option_line(6, "Spells", search_options);
        trie_search_option_line(7, "Features", search_options);
        trie_search_option_line(8, "Choosables", search_options);
        ImGui::TreePop();
    }
    if (old_trie_search_options != search_options) {
        search_changed = true;
    }

    if (search_changed) {
        session.set_trie_search(search_query, search_options);
    }
    ImGui::Separator();
    if (search_query.empty()) {
        ImGui::End();
        return;
    }
    size_t search_result_count = session.get_search_result_count();
    if (search_result_count == 0) {
        ImGui::Text("No results. Please broaden your search.");
        ImGui::End();
        return;
    }
    if (session.too_many_search_results()) {
        ImGui::Text("Too many results (%ld). Please refine your search.", search_result_count);
        ImGui::End();
        return;
    }

    if (ImGui::BeginChild("Search Results", ImVec2(-FLT_MIN, -FLT_MIN))) {
        std::vector<std::string> search_result_strings = session.get_trie_search_result_strings();
        for (size_t i = 0; i < search_result_count; ++i) {
            if (ImGui::Selectable(search_result_strings[i].c_str(), false)) {
                session.open_trie_search_result(i);
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();
}
