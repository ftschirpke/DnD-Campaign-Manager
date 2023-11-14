#include <dnd_config.hpp>

#include "advanced_search_window.hpp"

#include <algorithm>
#include <cassert>
#include <string>
#include <variant>
#include <vector>

#include <imgui/imgui.h>

#include <core/searching/content_filters/character/character_filter.hpp>
#include <core/searching/content_filters/character_class/character_class_filter.hpp>
#include <core/searching/content_filters/character_race/character_race_filter.hpp>
#include <core/searching/content_filters/character_subclass/character_subclass_filter.hpp>
#include <core/searching/content_filters/character_subrace/character_subrace_filter.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/effects_provider/choosable_filter.hpp>
#include <core/searching/content_filters/effects_provider/feature_filter.hpp>
#include <core/searching/content_filters/item/item_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>
#include <core/session.hpp>
#include <gui/visitors/filters/filter_setting_visitor.hpp>

static constexpr ImVec2 cell_padding = ImVec2(5, 5);
static constexpr ImGuiTableFlags content_table_flags = ImGuiTableFlags_NoBordersInBodyUntilResize;
static constexpr float first_column_width = 250;
static constexpr float second_column_width = 250;
static constexpr float third_column_width = 250;
static constexpr float fourth_column_width = 250;
static constexpr float first_column_button_width = 162.5f;

static constexpr std::array<const char*, 10> content_filter_names = {
    "Any", "Characters", "Classes", "Subclasses", "Races", "Subraces", "Items", "Spells", "Features", "Choosables",
};

dnd::AdvancedSearchWindow::AdvancedSearchWindow(Session& session) : session(session), result_list() {}

void dnd::AdvancedSearchWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("Advanced Search");

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
    bool table_exists = ImGui::BeginTable("advanced_search_filters", 5, content_table_flags);
    if (!table_exists) {
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }
    ImGui::TableSetupColumn("Filter", ImGuiTableColumnFlags_WidthFixed, first_column_width);
    ImGui::TableSetupColumn("Subfilter Name", ImGuiTableColumnFlags_WidthFixed, second_column_width);
    ImGui::TableSetupColumn("Subfilter Type", ImGuiTableColumnFlags_WidthFixed, third_column_width);
    ImGui::TableSetupColumn("Subfilter Value", ImGuiTableColumnFlags_WidthFixed, fourth_column_width);
    ImGui::TableSetupColumn("Other");
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    ContentFilterVariant& filter = session.get_advanced_search_filter();
    const size_t idx = filter.index();
    size_t swap_to_idx = content_filter_names.size();
    if (ImGui::BeginCombo("##Content Filter Type", content_filter_names[idx])) {
        for (size_t i = 0; i < content_filter_names.size(); ++i) {
            bool is_selected = idx == i;
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            } else if (ImGui::Selectable(content_filter_names[i], is_selected)) {
                swap_to_idx = i;
            }
        }
        ImGui::EndCombo();
    }
    if (swap_to_idx != content_filter_names.size()) {
        switch (swap_to_idx) {
            case 0:
                filter = ContentPieceFilter();
                break;
            case 1:
                filter = CharacterFilter();
                break;
            case 2:
                filter = CharacterClassFilter();
                break;
            case 3:
                filter = CharacterSubclassFilter();
                break;
            case 4:
                filter = CharacterRaceFilter();
                break;
            case 5:
                filter = CharacterSubraceFilter();
                break;
            case 6:
                filter = ItemFilter();
                break;
            case 7:
                filter = SpellFilter();
                break;
            case 8:
                filter = FeatureFilter();
                break;
            case 9:
                filter = ChoosableFilter();
                break;
            default:
                assert(false);
        }
    }

    std::visit(filter_setting_visitor, filter);
    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    if (ImGui::Button("Clear Filter", ImVec2(first_column_button_width, 0))) {
        std::visit([](ContentFilter& filter) { filter.clear(); }, filter);
    }

    ImGui::EndTable();
    ImGui::PopStyleVar();

    ImGui::Separator();
    ImGui::Spacing();
    if (ImGui::Button("Search", ImVec2(first_column_button_width, 0))) {
        session.start_advanced_search();
    }
    if (session.advanced_search_results_available()) {
        result_list = session.get_advanced_search_result_strings();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear Results", ImVec2(first_column_button_width, 0))) {
        result_list.clear();
    }
    ImGui::Spacing();
    ImGui::Separator();

    size_t i = 0;
    for (const std::string& result : result_list) {
        if (ImGui::Selectable(result.c_str(), false)) {
            session.open_advanced_search_result(i);
        }
        ++i;
    }

    ImGui::End();
}
