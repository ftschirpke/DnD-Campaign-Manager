#include <dnd_config.hpp>

#include "core/searching/content_filters/selection_filter.hpp"
#include "filter_setting_visitor.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <utility>

#include <fmt/format.h>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>
#include <core/visitors/filters/content_filter_visitor.hpp>

// TODO: Implement the following:
/* void operator()(const ContentPieceFilter& content_piece_filter) override; */
/* void operator()(const CharacterFilter& character_filter) override; */
/* void operator()(const CharacterClassFilter& character_class_filter) override; */
/* void operator()(const CharacterSubclassFilter& character_subclass_filter) override; */
/* void operator()(const CharacterRaceFilter& character_race_filter) override; */
/* void operator()(const CharacterSubraceFilter& character_subrace_filter) override; */
/* void operator()(const ItemFilter& item_filter) override; */
/* void operator()(const SpellFilter& spell_filter) override; */
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
    if (ImGui::Button("Remove")) {
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
    if (ImGui::Button("Remove")) {
        filter.clear();
    }
    ImGui::TableNextRow();
}

static constexpr std::array<std::pair<const char*, dnd::NumberFilterType>, 7> number_filter_types = {
    std::make_pair("None", dnd::NumberFilterType::NONE),
    std::make_pair("Equal", dnd::NumberFilterType::EQUAL),
    std::make_pair("Not Equal", dnd::NumberFilterType::NOT_EQUAL),
    std::make_pair("Less Than", dnd::NumberFilterType::LESS_THAN),
    std::make_pair("Less Than or Equal", dnd::NumberFilterType::LESS_THAN_OR_EQUAL),
    std::make_pair("Greater Than", dnd::NumberFilterType::GREATER_THAN),
    std::make_pair("Greater Than or Equal", dnd::NumberFilterType::GREATER_THAN_OR_EQUAL),
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
    if (ImGui::Button("Remove")) {
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
    if (ImGui::Button("Remove")) {
        filter.clear();
    }
    ImGui::TableNextRow();
}

void dnd::FilterSettingVisitor::operator()(dnd::ContentPieceFilter& content_piece_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    NameFilterVariant& name_filter = content_piece_filter.get_name_filter();
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
    visit_string_filter("Description", content_piece_filter.get_description_filter());
    visit_bool_filter("Is Sourcebook", content_piece_filter.get_is_sourcebook_filter());

    ImGui::TableSetColumnIndex(1);
    if (!content_piece_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        if (!content_piece_filter.has_name_filter() && ImGui::MenuItem("Name")) {
            content_piece_filter.get_name_filter().emplace<dnd::StringFilter>().set_type(dnd::StringFilterType::EQUAL);
            ImGui::CloseCurrentPopup();
        }
        if (!content_piece_filter.has_description_filter() && ImGui::MenuItem("Description")) {
            content_piece_filter.get_description_filter().set_type(dnd::StringFilterType::CONTAINS);
            ImGui::CloseCurrentPopup();
        }
        if (!content_piece_filter.has_is_sourcebook_filter() && ImGui::MenuItem("Is Sourcebook")) {
            content_piece_filter.get_is_sourcebook_filter().set_type(dnd::BoolFilterType::IS_TRUE);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void dnd::FilterSettingVisitor::operator()(dnd::SpellFilter& spell_filter) {
    DND_MEASURE_FUNCTION();
    ImGui::TableSetColumnIndex(1);
    NameFilterVariant& name_filter = spell_filter.get_name_filter();
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
    visit_string_filter("Description", spell_filter.get_description_filter());
    visit_bool_filter("Is Sourcebook", spell_filter.get_is_sourcebook_filter());
    visit_bool_filter("Verbal Component", spell_filter.get_verbal_component_filter());
    visit_bool_filter("Somatic Component", spell_filter.get_somatic_component_filter());
    visit_bool_filter("Material Component", spell_filter.get_material_component_filter());
    visit_number_filter("Level", spell_filter.get_level_filter());
    visit_selection_filter("Magic School", spell_filter.get_magic_school_filter());
    visit_bool_filter("Ritual", spell_filter.get_ritual_filter());
    visit_string_filter("Casting Time", spell_filter.get_casting_time_filter());
    visit_string_filter("Range", spell_filter.get_range_filter());
    visit_string_filter("Duration", spell_filter.get_duration_filter());
    visit_selection_filter("Classes", spell_filter.get_classes_filter());

    ImGui::TableSetColumnIndex(1);
    if (!spell_filter.has_all_filters() && ImGui::Button("Add Value Filter")) {
        ImGui::OpenPopup("value_filter_popup");
    }
    if (ImGui::BeginPopup("value_filter_popup")) {
        if (!spell_filter.has_name_filter() && ImGui::MenuItem("Name")) {
            spell_filter.get_name_filter().emplace<dnd::StringFilter>().set_type(dnd::StringFilterType::EQUAL);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_description_filter() && ImGui::MenuItem("Description")) {
            spell_filter.get_description_filter().set_type(dnd::StringFilterType::CONTAINS);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_is_sourcebook_filter() && ImGui::MenuItem("Is Sourcebook")) {
            spell_filter.get_is_sourcebook_filter().set_type(dnd::BoolFilterType::IS_TRUE);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_verbal_component_filter() && ImGui::MenuItem("Verbal Component")) {
            spell_filter.get_verbal_component_filter().set_type(dnd::BoolFilterType::IS_TRUE);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_somatic_component_filter() && ImGui::MenuItem("Somatic Component")) {
            spell_filter.get_somatic_component_filter().set_type(dnd::BoolFilterType::IS_TRUE);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_material_component_filter() && ImGui::MenuItem("Material Component")) {
            spell_filter.get_material_component_filter().set_type(dnd::BoolFilterType::IS_TRUE);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_level_filter() && ImGui::MenuItem("Level")) {
            spell_filter.get_level_filter().set_type(dnd::NumberFilterType::EQUAL);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_magic_school_filter() && ImGui::MenuItem("Magic School")) {
            spell_filter.get_magic_school_filter().set_type(dnd::SelectionFilterType::IS_IN);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_ritual_filter() && ImGui::MenuItem("Ritual")) {
            spell_filter.get_ritual_filter().set_type(dnd::BoolFilterType::IS_TRUE);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_casting_time_filter() && ImGui::MenuItem("Casting Time")) {
            spell_filter.get_casting_time_filter().set_type(dnd::StringFilterType::CONTAINS);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_range_filter() && ImGui::MenuItem("Range")) {
            spell_filter.get_range_filter().set_type(dnd::StringFilterType::CONTAINS);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_duration_filter() && ImGui::MenuItem("Duration")) {
            spell_filter.get_duration_filter().set_type(dnd::StringFilterType::CONTAINS);
            ImGui::CloseCurrentPopup();
        }
        if (!spell_filter.has_classes_filter() && ImGui::MenuItem("Classes")) {
            spell_filter.get_classes_filter().set_type(dnd::SelectionFilterType::IS_IN);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
