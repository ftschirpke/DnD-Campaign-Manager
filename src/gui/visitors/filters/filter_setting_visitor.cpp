#include <dnd_config.hpp>

#include "filter_setting_visitor.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <utility>

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/character/character_filter.hpp>
#include <core/searching/content_filters/character_class/character_class_filter.hpp>
#include <core/searching/content_filters/character_race/character_race_filter.hpp>
#include <core/searching/content_filters/character_subclass/character_subclass_filter.hpp>
#include <core/searching/content_filters/character_subrace/character_subrace_filter.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/item/item_filter.hpp>
#include <core/searching/content_filters/selection_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>
#include <core/visitors/filters/content_filter_visitor.hpp>

// TODO: Implement the following:
/* void operator()(const FeatureFilter& feature_filter) override; */
/* void operator()(const ChoosableFilter& choosable_filter) override; */

static constexpr ImGuiComboFlags combo_flags = ImGuiComboFlags_PopupAlignLeft;

template <typename S, typename T, size_t K>
static size_t find_index(const std::array<std::pair<S, T>, K>& array, const T& value) {
    for (size_t i = 0; i < array.size(); ++i) {
        if (array[i].second == value) {
            return i;
        }
    }
    return array.size();
}

static constexpr std::array<std::pair<const char*, dnd::BoolFilterType>, 3> bool_filter_types = {
    std::make_pair("None", dnd::BoolFilterType::NONE),
    std::make_pair("Is True", dnd::BoolFilterType::IS_TRUE),
    std::make_pair("Is False", dnd::BoolFilterType::IS_FALSE),
};

static void visit_bool_filter(const char* name, dnd::BoolFilter& filter) {
    DND_MEASURE_FUNCTION();
    dnd::BoolFilterType type = filter.get_type();
    if (type == dnd::BoolFilterType::NONE) {
        return;
    }
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", name);
    ImGui::TableSetColumnIndex(2);
    size_t idx = find_index(bool_filter_types, type);
    assert(idx < bool_filter_types.size());
    std::string label = fmt::format("##{}", name);
    if (ImGui::BeginCombo(label.c_str(), bool_filter_types[idx].first, combo_flags)) {
        for (size_t i = 1; i < bool_filter_types.size(); ++i) {
            bool is_selected = idx == i;
            if (ImGui::Selectable(bool_filter_types[i].first, is_selected)) {
                filter.set_type(bool_filter_types[i].second);
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::TableSetColumnIndex(4);
    std::string remove_label = fmt::format("Remove##{}", name);
    if (ImGui::Button(remove_label.c_str())) {
        filter.clear();
    }
    ImGui::TableNextRow();
}

static constexpr std::array<std::pair<const char*, dnd::StringFilterType>, 9> string_filter_types = {
    std::make_pair("None", dnd::StringFilterType::NONE),
    std::make_pair("Equal", dnd::StringFilterType::EQUAL),
    std::make_pair("Not Equal", dnd::StringFilterType::NOT_EQUAL),
    std::make_pair("Contains", dnd::StringFilterType::CONTAINS),
    std::make_pair("Not Contains", dnd::StringFilterType::NOT_CONTAINS),
    std::make_pair("Starts With", dnd::StringFilterType::STARTS_WITH),
    std::make_pair("Not Starts With", dnd::StringFilterType::NOT_STARTS_WITH),
    std::make_pair("Ends With", dnd::StringFilterType::ENDS_WITH),
    std::make_pair("Not Ends With", dnd::StringFilterType::NOT_ENDS_WITH),
};

static void visit_string_filter(const char* name, dnd::StringFilter& filter) {
    DND_MEASURE_FUNCTION();
    dnd::StringFilterType type = filter.get_type();
    if (type == dnd::StringFilterType::NONE) {
        return;
    }
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", name);
    ImGui::TableSetColumnIndex(2);
    size_t idx = find_index(string_filter_types, type);
    assert(idx < string_filter_types.size());
    std::string label = fmt::format("##{}", name);
    if (ImGui::BeginCombo(label.c_str(), string_filter_types[idx].first, combo_flags)) {
        for (size_t i = 1; i < string_filter_types.size(); ++i) {
            bool is_selected = idx == i;
            if (ImGui::Selectable(string_filter_types[i].first, is_selected)) {
                filter.set_type(string_filter_types[i].second);
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::TableSetColumnIndex(3);
    std::string value_label = fmt::format("##{} value", name);
    ImGui::InputText(value_label.c_str(), &filter.get_value());
    ImGui::TableSetColumnIndex(4);
    std::string remove_label = fmt::format("Remove##{}", name);
    if (ImGui::Button(remove_label.c_str())) {
        filter.clear();
    }
    ImGui::TableNextRow();
}

static constexpr std::array<std::pair<const char*, dnd::NumberFilterType>, 7> number_filter_types = {
    std::make_pair("None", dnd::NumberFilterType::NONE),
    std::make_pair("=", dnd::NumberFilterType::EQUAL),
    std::make_pair("!=", dnd::NumberFilterType::NOT_EQUAL),
    std::make_pair("<", dnd::NumberFilterType::LESS_THAN),
    std::make_pair("<=", dnd::NumberFilterType::LESS_THAN_OR_EQUAL),
    std::make_pair(">", dnd::NumberFilterType::GREATER_THAN),
    std::make_pair(">=", dnd::NumberFilterType::GREATER_THAN_OR_EQUAL),
};

template <typename T>
requires std::is_arithmetic_v<T>
static ImGuiDataType_ number_data_type();

template <>
ImGuiDataType_ number_data_type<int>() {
    return ImGuiDataType_S32;
}

/* TODO: Uncomment when using floats
template <>
ImGuiDataType_ number_data_type<float>() {
    return ImGuiDataType_Float;
}
*/

template <typename T>
requires std::is_arithmetic_v<T>
static void visit_number_filter(const char* name, dnd::NumberFilter<T>& filter) {
    DND_MEASURE_FUNCTION();
    dnd::NumberFilterType type = filter.get_type();
    if (type == dnd::NumberFilterType::NONE) {
        return;
    }
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", name);
    ImGui::TableSetColumnIndex(2);
    size_t idx = find_index(number_filter_types, type);
    assert(idx < number_filter_types.size());
    std::string label = fmt::format("##{}", name);
    if (ImGui::BeginCombo(label.c_str(), number_filter_types[idx].first, combo_flags)) {
        for (size_t i = 1; i < number_filter_types.size(); ++i) {
            bool is_selected = idx == i;
            if (ImGui::Selectable(number_filter_types[i].first, is_selected)) {
                filter.set_type(number_filter_types[i].second);
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::TableSetColumnIndex(3);
    std::string value_label = fmt::format("##{} value", name);
    T set_value = filter.get_value();
    if (ImGui::InputScalar(value_label.c_str(), number_data_type<T>(), &set_value)) {
        filter.set_value(set_value);
    }
    ImGui::TableSetColumnIndex(4);
    std::string remove_label = fmt::format("Remove##{}", name);
    if (ImGui::Button(remove_label.c_str())) {
        filter.clear();
    }
    ImGui::TableNextRow();
}

static constexpr std::array<std::pair<const char*, dnd::SelectionFilterType>, 3> selection_filter_types = {
    std::make_pair("None", dnd::SelectionFilterType::NONE),
    std::make_pair("Is In", dnd::SelectionFilterType::IS_IN),
    std::make_pair("Is Not In", dnd::SelectionFilterType::IS_NOT_IN),
};

template <typename T>
static void visit_selection_filter(const char* name, dnd::SelectionFilter<T>& filter) {
    DND_MEASURE_FUNCTION();
    dnd::SelectionFilterType type = filter.get_type();
    if (type == dnd::SelectionFilterType::NONE) {
        return;
    }
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", name);
    ImGui::TableSetColumnIndex(2);
    size_t idx = find_index(selection_filter_types, type);
    assert(idx < dnd::selection_filter_types.size());
    std::string label = fmt::format("##{}", name);
    if (ImGui::BeginCombo(label.c_str(), selection_filter_types[idx].first, combo_flags)) {
        for (size_t i = 1; i < selection_filter_types.size(); ++i) {
            bool is_selected = idx == i;
            if (ImGui::Selectable(selection_filter_types[i].first, is_selected)) {
                filter.set_type(selection_filter_types[i].second);
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::TableSetColumnIndex(3);
    ImGui::Text("Coming soon"); // TODO: Implement selection creation
    ImGui::TableSetColumnIndex(4);
    std::string remove_label = fmt::format("Remove##{}", name);
    if (ImGui::Button(remove_label.c_str())) {
        filter.clear();
    }
    ImGui::TableNextRow();
}

static void visit_content_piece_filter(dnd::ContentPieceFilter& content_piece_filter) {
    dnd::NameFilterVariant& name_filter = content_piece_filter.name_filter;
    switch (name_filter.index()) {
        case 0:
            visit_string_filter("Name", std::get<0>(name_filter));
            break;
        case 1:
            visit_selection_filter("Name", std::get<1>(name_filter));
            break;
        default:
            break;
    }
    visit_string_filter("Description", content_piece_filter.description_filter);
    visit_bool_filter("Is Sourcebook", content_piece_filter.is_sourcebook_filter);
}

static void bool_menu_item(const char* name, dnd::BoolFilter& filter, dnd::BoolFilterType default_type) {
    if (!filter.is_set() && ImGui::MenuItem(name)) {
        filter.set_type(default_type);
        ImGui::CloseCurrentPopup();
    }
}

static void string_menu_item(const char* name, dnd::StringFilter& filter, dnd::StringFilterType default_type) {
    if (!filter.is_set() && ImGui::MenuItem(name)) {
        filter.set_type(default_type);
        ImGui::CloseCurrentPopup();
    }
}

template <typename T>
requires std::is_arithmetic_v<T>
static void number_menu_item(const char* name, dnd::NumberFilter<T>& filter, dnd::NumberFilterType default_type) {
    if (!filter.is_set() && ImGui::MenuItem(name)) {
        filter.set_type(default_type);
        ImGui::CloseCurrentPopup();
    }
}

template <typename T>
static void selection_menu_item(
    const char* name, dnd::SelectionFilter<T>& filter, dnd::SelectionFilterType default_type
) {
    if (!filter.is_set() && ImGui::MenuItem(name)) {
        filter.set_type(default_type);
        ImGui::CloseCurrentPopup();
    }
}

static void content_piece_filter_menu_items(dnd::ContentPieceFilter& content_piece_filter) {
    bool name_filter_set = std::visit(
        [](const auto& filter) noexcept { return filter.is_set(); }, content_piece_filter.name_filter
    );
    if (!name_filter_set && ImGui::MenuItem("Name")) {
        content_piece_filter.name_filter.emplace<dnd::StringFilter>().set_type(dnd::StringFilterType::EQUAL);
        ImGui::CloseCurrentPopup();
    }
    string_menu_item("Description", content_piece_filter.description_filter, dnd::StringFilterType::CONTAINS);
    bool_menu_item("Is Sourcebook", content_piece_filter.is_sourcebook_filter, dnd::BoolFilterType::IS_TRUE);
}

void dnd::FilterSettingVisitor::operator()(ContentPieceFilter& content_piece_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);

    ImGui::TableSetColumnIndex(1);
    if (!content_piece_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        content_piece_filter_menu_items(content_piece_filter);
        ImGui::EndPopup();
    }
}

void dnd::FilterSettingVisitor::operator()(CharacterFilter& character_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    visit_content_piece_filter(character_filter);
    visit_number_filter("Level", character_filter.level_filter);
    visit_number_filter("XP", character_filter.xp_filter);

    ImGui::TableSetColumnIndex(1);
    if (!character_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        content_piece_filter_menu_items(character_filter);
        number_menu_item("Level", character_filter.level_filter, dnd::NumberFilterType::EQUAL);
        number_menu_item("XP", character_filter.xp_filter, dnd::NumberFilterType::GREATER_THAN_OR_EQUAL);
        ImGui::EndPopup();
    }
}

void dnd::FilterSettingVisitor::operator()(CharacterClassFilter& class_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    visit_content_piece_filter(class_filter);
    visit_bool_filter("Has Spellcasting", class_filter.has_spellcasting_filter);

    ImGui::TableSetColumnIndex(1);
    if (!class_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        content_piece_filter_menu_items(class_filter);
        bool_menu_item("Has Spellcasting", class_filter.has_spellcasting_filter, dnd::BoolFilterType::IS_TRUE);
        ImGui::EndPopup();
    }
}

void dnd::FilterSettingVisitor::operator()(CharacterSubclassFilter& subclass_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    visit_content_piece_filter(subclass_filter);
    visit_bool_filter("Has Spellcasting", subclass_filter.has_spellcasting_filter);

    ImGui::TableSetColumnIndex(1);
    if (!subclass_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        content_piece_filter_menu_items(subclass_filter);
        bool_menu_item("Has Spellcasting", subclass_filter.has_spellcasting_filter, dnd::BoolFilterType::IS_TRUE);
        ImGui::EndPopup();
    }
}

void dnd::FilterSettingVisitor::operator()(CharacterRaceFilter& race_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    visit_content_piece_filter(race_filter);
    visit_bool_filter("Has Subraces", race_filter.has_subraces_filter);

    ImGui::TableSetColumnIndex(1);
    if (!race_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        content_piece_filter_menu_items(race_filter);
        bool_menu_item("Has Subraces", race_filter.has_subraces_filter, dnd::BoolFilterType::IS_TRUE);
        ImGui::EndPopup();
    }
}

void dnd::FilterSettingVisitor::operator()(CharacterSubraceFilter& subrace_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    visit_content_piece_filter(subrace_filter);

    ImGui::TableSetColumnIndex(1);
    if (!subrace_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        content_piece_filter_menu_items(subrace_filter);
        ImGui::EndPopup();
    }
}

void dnd::FilterSettingVisitor::operator()(ItemFilter& item_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    visit_content_piece_filter(item_filter);
    visit_string_filter("Cosmetic Description", item_filter.cosmetic_description_filter);
    visit_bool_filter("Requires Attunement", item_filter.attunement_filter);

    ImGui::TableSetColumnIndex(1);
    if (!item_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        content_piece_filter_menu_items(item_filter);
        string_menu_item(
            "Cosmetic Description", item_filter.cosmetic_description_filter, dnd::StringFilterType::CONTAINS
        );
        bool_menu_item("Requires Attunement", item_filter.attunement_filter, dnd::BoolFilterType::IS_TRUE);
        ImGui::EndPopup();
    }
}

void dnd::FilterSettingVisitor::operator()(SpellFilter& spell_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    visit_content_piece_filter(spell_filter);
    visit_bool_filter("Verbal Component", spell_filter.verbal_component_filter);
    visit_bool_filter("Somatic Component", spell_filter.somatic_component_filter);
    visit_bool_filter("Material Component", spell_filter.material_component_filter);
    visit_number_filter("Level", spell_filter.level_filter);
    visit_selection_filter("Magic School", spell_filter.magic_school_filter);
    visit_bool_filter("Ritual", spell_filter.ritual_filter);
    visit_string_filter("Casting Time", spell_filter.casting_time_filter);
    visit_string_filter("Range", spell_filter.range_filter);
    visit_string_filter("Duration", spell_filter.duration_filter);
    visit_selection_filter("Classes", spell_filter.classes_filter);

    ImGui::TableSetColumnIndex(1);
    if (!spell_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        content_piece_filter_menu_items(spell_filter);
        bool_menu_item("Verbal Component", spell_filter.verbal_component_filter, dnd::BoolFilterType::IS_TRUE);
        bool_menu_item("Somatic Component", spell_filter.somatic_component_filter, dnd::BoolFilterType::IS_TRUE);
        bool_menu_item("Material Component", spell_filter.material_component_filter, dnd::BoolFilterType::IS_TRUE);
        number_menu_item("Level", spell_filter.level_filter, dnd::NumberFilterType::EQUAL);
        selection_menu_item("Magic School", spell_filter.magic_school_filter, dnd::SelectionFilterType::IS_IN);
        bool_menu_item("Ritual", spell_filter.ritual_filter, dnd::BoolFilterType::IS_TRUE);
        string_menu_item("Casting Time", spell_filter.casting_time_filter, dnd::StringFilterType::CONTAINS);
        string_menu_item("Range", spell_filter.range_filter, dnd::StringFilterType::CONTAINS);
        string_menu_item("Duration", spell_filter.duration_filter, dnd::StringFilterType::CONTAINS);
        selection_menu_item("Classes", spell_filter.classes_filter, dnd::SelectionFilterType::IS_IN);
        ImGui::EndPopup();
    }
}
