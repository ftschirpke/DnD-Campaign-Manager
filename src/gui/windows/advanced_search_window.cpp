#include <dnd_config.hpp>

#include "advanced_search_window.hpp"

#include <cassert>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <imgui/imgui.h>

#include <core/searching/advanced_search/advanced_content_search.hpp>
#include <core/searching/content_filters/character/character_filter.hpp>
#include <core/searching/content_filters/class/class_filter.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/effects_provider/choosable_filter.hpp>
#include <core/searching/content_filters/effects_provider/feature_filter.hpp>
#include <core/searching/content_filters/item/item_filter.hpp>
#include <core/searching/content_filters/species/species_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>
#include <core/searching/content_filters/subclass/subclass_filter.hpp>
#include <core/searching/content_filters/subspecies/subspecies_filter.hpp>
#include <core/session.hpp>
#include <gui/visitors/filters/filter_setting_visitor.hpp>

namespace dnd {

static constexpr ImVec2 cell_padding = ImVec2(5, 5);
static constexpr ImGuiTableFlags content_table_flags = ImGuiTableFlags_NoBordersInBodyUntilResize;
static constexpr float first_column_width = 250;
static constexpr float second_column_width = 250;
static constexpr float third_column_width = 250;
static constexpr float fourth_column_width = 250;
static constexpr float first_column_button_width = 162.5f;

static constexpr std::array<const char*, 10> content_filter_names = {
    "Any", "Characters", "Classes", "Subclasses", "Species", "Subspecies", "Items", "Spells", "Features", "Choosables",
};

static ContentFilterVariant get_filter_from_filter_index(const Content& content, size_t idx) {
    switch (idx) {
        case 0:
            return ContentPieceFilter(content);
        case 1:
            return CharacterFilter(content);
        case 2:
            return ClassFilter(content);
        case 3:
            return SubclassFilter(content);
        case 4:
            return SpeciesFilter(content);
        case 5:
            return SubspeciesFilter(content);
        case 6:
            return ItemFilter(content);
        case 7:
            return SpellFilter(content);
        case 8:
            return FeatureFilter(content);
        case 9:
            return ChoosableFilter(content);
    }
    std::unreachable();
}

AdvancedSearchWindow::AdvancedSearchWindow(Session& session) : session(session), result_list() {}

void AdvancedSearchWindow::render() {
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
        filter = get_filter_from_filter_index(session.get_content(), swap_to_idx);
        session.set_advanced_search_filter(std::move(filter));
    }

    std::visit(filter_setting_visitor, filter);
    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    if (ImGui::Button("Clear Filter", ImVec2(first_column_button_width, 0))) {
        dispatch(filter, auto& f, f.clear());
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

} // namespace dnd
