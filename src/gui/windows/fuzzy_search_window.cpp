#include <dnd_config.hpp>

#include "fuzzy_search_window.hpp"

#include <string>

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <core/session.hpp>

namespace dnd {

FuzzySearchWindow::FuzzySearchWindow(Session& session) : session(session), search_query(), search_options() {
    search_options.set_all(true);
}

void FuzzySearchWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Simple Search");

    bool search_changed = false;
    if (ImGui::InputText("Search", &search_query, ImGuiInputTextFlags_EscapeClearsAll, nullptr, nullptr)) {
        search_changed = true;
    }
    FuzzySearchOptions old_fuzzy_search_options = search_options;
    if (ImGui::TreeNode("Search options")) {
        if (ImGui::Button("All", ImVec2(200, 0))) {
            search_options.set_all(true);
        }

        if (ImGui::Button("Only##Characters")) {
            search_options.set_all(false);
            search_options.search_characters = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Characters", &search_options.search_characters);

        if (ImGui::Button("Only##Species")) {
            search_options.set_all(false);
            search_options.search_species = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Species", &search_options.search_species);

        if (ImGui::Button("Only##Subspecies")) {
            search_options.set_all(false);
            search_options.search_subspecies = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Subspecies", &search_options.search_subspecies);

        if (ImGui::Button("Only##Classes")) {
            search_options.set_all(false);
            search_options.search_classes = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Classes", &search_options.search_classes);

        if (ImGui::Button("Only##Subclasses")) {
            search_options.set_all(false);
            search_options.search_subclasses = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Subclasses", &search_options.search_subclasses);

        if (ImGui::Button("Only##Items")) {
            search_options.set_all(false);
            search_options.search_items = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Items", &search_options.search_items);

        if (ImGui::Button("Only##Spells")) {
            search_options.set_all(false);
            search_options.search_spells = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Spells", &search_options.search_spells);

        if (ImGui::Button("Only##Features")) {
            search_options.set_all(false);
            search_options.search_features = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Features", &search_options.search_features);

        if (ImGui::Button("Only##Choosables")) {
            search_options.set_all(false);
            search_options.search_choosables = true;
        }
        ImGui::SameLine();
        ImGui::Checkbox("Choosables", &search_options.search_choosables);

        ImGui::TreePop();
    }
    if (old_fuzzy_search_options != search_options) {
        search_changed = true;
    }

    if (search_changed) {
        session.set_fuzzy_search(search_query, search_options);
    }
    ImGui::Separator();

    if (search_query.empty()) {
        ImGui::End();
        return;
    }
    size_t search_result_count = session.get_fuzzy_search_result_count();
    if (search_result_count == 0) {
        ImGui::Text("No results. Please broaden your search.");
        ImGui::End();
        return;
    }
    if (session.too_many_fuzzy_search_results()) {
        ImGui::Text("Too many results (%ld). Please refine your search.", search_result_count);
        ImGui::End();
        return;
    }

    if (ImGui::BeginChild("Search Results", ImVec2(-FLT_MIN, -FLT_MIN))) {
        std::vector<std::string> search_result_strings = session.get_fuzzy_search_result_strings();
        for (size_t i = 0; i < search_result_count; ++i) {
            if (ImGui::Selectable(search_result_strings[i].c_str(), false)) {
                session.open_fuzzy_search_result(i);
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

} // namespace dnd
