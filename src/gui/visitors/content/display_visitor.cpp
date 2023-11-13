#include <dnd_config.hpp>

#include "display_visitor.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <imgui/imgui.h>

#include <core/basic_mechanics/dice.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/output/string_formatting/formats/format.hpp>
#include <core/output/string_formatting/string_formatter.hpp>
#include <core/visitors/content/content_visitor.hpp>

static const ImVec2 cell_padding = ImVec2(5, 5);
static constexpr ImGuiTableFlags content_table_flags = ImGuiTableFlags_NoBordersInBodyUntilResize;
static const float first_column_width = 150;

static constexpr ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
                                               | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;

static void label(const char* label) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", label);
    ImGui::TableSetColumnIndex(1);
}

static void wrapped_label(const char* label) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextWrapped("%s", label);
    ImGui::TableSetColumnIndex(1);
}

static void source(const dnd::ContentPiece& content_piece) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Source:");
    ImGui::TableSetColumnIndex(1);
    ImGui::TextWrapped(
        "%s / %s / %s", content_piece.get_source_info().get_source_group_name().c_str(),
        content_piece.get_source_info().get_source_type_name().c_str(),
        content_piece.get_source_info().get_source_name().c_str()
    );
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Separator();
    ImGui::TableSetColumnIndex(1);
    ImGui::Separator();
}

static void begin_content_table(const dnd::ContentPiece& content_piece) {
    std::string table_id = content_piece.get_name() + "_table";
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);
    ImGui::BeginTable(table_id.c_str(), 2, content_table_flags);
    ImGui::TableSetupColumn("Labels", ImGuiTableColumnFlags_WidthFixed, first_column_width);
    ImGui::TableSetupColumn("Values");
}

static void end_content_table() {
    ImGui::EndTable();
    ImGui::PopStyleVar();
}

static void display_formatted_text(const std::string& formatted_text) {
    static dnd::DisplayFormatVisitor display_format_visitor(table_flags);
    static dnd::StringFormatter string_formatter(false);
    std::vector<std::unique_ptr<dnd::Format>> text_formats = string_formatter.parse_formats(formatted_text);
    for (auto it = text_formats.begin(); it != text_formats.end(); ++it) {
        (*it)->accept(display_format_visitor);
    }
}

template <typename T>
static void list_features(dnd::DisplayVisitor& visitor, const std::vector<T>& features) {
    if (features.empty()) {
        ImGui::Text("None");
        return;
    }
    for (const T& feature : features) {
        ImGui::Separator();
        if (ImGui::TreeNode(feature.get_name().c_str())) {
            ImGui::Separator();
            visitor.visit(feature);
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
}

void dnd::DisplayVisitor::visit(const Character& character) {
    begin_content_table(character);

    label("Type:");
    ImGui::Text("Character");
    source(character);
    label("Level:");
    ImGui::Text("%d", character.get_progression().get_level());
    label("XP:");
    ImGui::Text("%d", character.get_progression().get_xp());

    // TODO: display stats (again)

    label("Race:");
    const CharacterRace* race_ptr = character.get_basis().get_race();
    assert(race_ptr != nullptr);
    if (ImGui::CollapsingHeader(race_ptr->get_name().c_str())) {
        visit(*race_ptr);
    }

    const CharacterSubrace* const subrace_ptr = character.get_basis().get_subrace();
    if (subrace_ptr != nullptr) {
        label("Subrace:");
        if (ImGui::CollapsingHeader(character.get_basis().get_subrace()->get_name().c_str())) {
            visit(*subrace_ptr);
        }
    }

    label("Class:");
    const CharacterClass* const class_ptr = character.get_basis().get_class();
    assert(class_ptr != nullptr);
    if (ImGui::CollapsingHeader(class_ptr->get_name().c_str())) {
        visit(*class_ptr);
    }

    const CharacterSubclass* const subclass_ptr = character.get_basis().get_subclass();
    if (subclass_ptr != nullptr) {
        label("Subclass:");
        if (ImGui::CollapsingHeader(character.get_basis().get_subclass()->get_name().c_str())) {
            visit(*subclass_ptr);
        }
    }

    label("Features:");
    list_features<Feature>(*this, character.get_features());

    end_content_table();
}

void dnd::DisplayVisitor::visit(const CharacterClass& character_class) {
    begin_content_table(character_class);

    label("Type:");
    ImGui::Text("Class");
    source(character_class);
    label("Hit Die:");
    ImGui::Text("%s", dice_to_string(character_class.get_hit_dice()).c_str());
    label("Feat Levels:");
    std::string feat_level_str = fmt::format(
        "{}", fmt::join(character_class.get_important_levels().get_feat_levels(), ", ")
    );
    ImGui::Text("%s", feat_level_str.c_str());
    label("Subclass Level:");
    ImGui::Text("%d", character_class.get_important_levels().get_subclass_level());
    label("Features:");
    list_features<ClassFeature>(*this, character_class.get_features());

    end_content_table();
}

void dnd::DisplayVisitor::visit(const CharacterSubclass& character_subclass) {
    begin_content_table(character_subclass);

    label("Type:");
    ImGui::Text("Subclass");
    source(character_subclass);
    label("Class name:");
    ImGui::Text("%s", character_subclass.get_class()->get_name().c_str());
    label("Features:");
    list_features<ClassFeature>(*this, character_subclass.get_features());

    end_content_table();
}

void dnd::DisplayVisitor::visit(const CharacterRace& character_race) {
    begin_content_table(character_race);

    label("Type:");
    ImGui::Text("Race");
    source(character_race);
    const char* has_subraces_cstr = character_race.has_subraces() ? "yes" : "no";
    label("Has Subraces:");
    ImGui::Text("%s", has_subraces_cstr);
    label("Features:");
    list_features<Feature>(*this, character_race.get_features());

    end_content_table();
}

void dnd::DisplayVisitor::visit(const CharacterSubrace& character_subrace) {
    begin_content_table(character_subrace);

    label("Type:");
    ImGui::Text("Subrace");
    source(character_subrace);
    label("Race name:");
    ImGui::Text("%s", character_subrace.get_race()->get_name().c_str());
    label("Features:");
    list_features<Feature>(*this, character_subrace.get_features());

    end_content_table();
}

void dnd::DisplayVisitor::visit(const Item& item) {
    begin_content_table(item);

    label("Type:");
    ImGui::Text("Item");
    source(item);
    label("Attunement");
    const char* attunement = item.requires_attunement() ? "required" : "not required";
    ImGui::Text("%s", attunement);
    label("Description:");
    display_formatted_text(item.get_description());
    if (!item.get_cosmetic_description().empty()) {
        wrapped_label("Cosmetic Description:");
        display_formatted_text(item.get_cosmetic_description());
    }

    end_content_table();
}

void dnd::DisplayVisitor::visit(const Spell& spell) {
    begin_content_table(spell);

    label("Type:");
    ImGui::Text("Spell");
    source(spell);
    label("Spell Type:");
    ImGui::Text("%s", spell.get_type().str().c_str());
    label("Ritual:");
    const char* ritual = spell.get_type().is_ritual() ? "yes" : "no";
    ImGui::Text("%s", ritual);
    label("Casting Time:");
    ImGui::Text("%s", spell.get_casting_time().c_str());
    label("Range:");
    ImGui::Text("%s", spell.get_range().c_str());
    label("Components:");
    ImGui::TextWrapped("%s", spell.get_components().str().c_str());
    label("Duration:");
    ImGui::Text("%s", spell.get_duration().c_str());

    label("Description:");
    display_formatted_text(spell.get_description());

    end_content_table();
}

void dnd::DisplayVisitor::visit(const Feature& feature) {
    begin_content_table(feature);

    label("Type:");
    ImGui::Text("Feature");
    source(feature);
    label("Description:");
    display_formatted_text(feature.get_description());

    end_content_table();
}

void dnd::DisplayVisitor::visit(const Choosable& choosable) {
    begin_content_table(choosable);

    label("Type:");
    ImGui::Text("Choosable Feature - %s", choosable.get_type().c_str());
    source(choosable);
    label("Description:");
    display_formatted_text(choosable.get_description());

    end_content_table();
}
