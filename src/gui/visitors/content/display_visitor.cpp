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
#include <core/models/class/class.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/output/string_formatting/formats/format.hpp>
#include <core/output/string_formatting/string_formatter.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

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

static void source(const ContentPiece& content_piece) {
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

static void begin_content_table(const ContentPiece& content_piece) {
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
    static DisplayFormatVisitor display_format_visitor(table_flags);
    static StringFormatter string_formatter(false);
    std::vector<std::unique_ptr<Format>> text_formats = string_formatter.parse_formats(formatted_text);
    for (auto it = text_formats.begin(); it != text_formats.end(); ++it) {
        (*it)->accept(display_format_visitor);
    }
}

template <typename T>
static void list_features(DisplayVisitor& visitor, const std::vector<T>& features) {
    if (features.empty()) {
        ImGui::Text("None");
        return;
    }
    for (const T& feature : features) {
        ImGui::Separator();
        if (ImGui::TreeNode(feature.get_name().c_str())) {
            ImGui::Separator();
            visitor(feature);
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
}

void DisplayVisitor::operator()(const Character& character) {
    begin_content_table(character);

    label("Type:");
    ImGui::Text("Character");
    source(character);
    label("Level:");
    ImGui::Text("%d", character.get_progression().get_level());
    label("XP:");
    ImGui::Text("%d", character.get_progression().get_xp());

    // TODO: display stats (again)

    label("Species:");
    const Species& species = character.get_feature_providers().get_species();
    if (ImGui::CollapsingHeader(species.get_name().c_str())) {
        operator()(species);
    }

    OptCRef<Subspecies> subspecies_optional = character.get_feature_providers().get_subspecies();
    if (subspecies_optional.has_value()) {
        label("Subspecies:");
        const Subspecies& subspecies = subspecies_optional.value();
        if (ImGui::CollapsingHeader(subspecies.get_name().c_str())) {
            operator()(subspecies);
        }
    }

    label("Class:");
    const Class& cls = character.get_feature_providers().get_class();
    if (ImGui::CollapsingHeader(cls.get_name().c_str())) {
        operator()(cls);
    }

    OptCRef<Subclass> subclass_optional = character.get_feature_providers().get_subclass();
    if (subclass_optional.has_value()) {
        label("Subclass:");
        const Subclass& subclass = subclass_optional.value();
        if (ImGui::CollapsingHeader(subclass.get_name().c_str())) {
            operator()(subclass);
        }
    }

    label("Features:");
    list_features<Feature>(*this, character.get_features());

    end_content_table();
}

void DisplayVisitor::operator()(const Class& cls) {
    begin_content_table(cls);

    label("Type:");
    ImGui::Text("Class");
    source(cls);
    label("Hit Die:");
    ImGui::Text("%s", dice_to_string(cls.get_hit_dice()).c_str());
    label("Feat Levels:");
    std::string feat_level_str = fmt::format("{}", fmt::join(cls.get_important_levels().get_feat_levels(), ", "));
    ImGui::Text("%s", feat_level_str.c_str());
    label("Subclass Level:");
    ImGui::Text("%d", cls.get_important_levels().get_subclass_level());
    label("Features:");
    list_features<ClassFeature>(*this, cls.get_features());

    end_content_table();
}

void DisplayVisitor::operator()(const Subclass& subclass) {
    begin_content_table(subclass);

    label("Type:");
    ImGui::Text("Subclass");
    source(subclass);
    label("Class name:");
    ImGui::Text("%s", subclass.get_class()->get_name().c_str());
    label("Features:");
    list_features<ClassFeature>(*this, subclass.get_features());

    end_content_table();
}

void DisplayVisitor::operator()(const Species& species) {
    begin_content_table(species);

    label("Type:");
    ImGui::Text("Species");
    source(species);
    const char* has_subspecies_cstr = species.has_subspecies() ? "yes" : "no";
    label("Has Subspecies:");
    ImGui::Text("%s", has_subspecies_cstr);
    label("Features:");
    list_features<Feature>(*this, species.get_features());

    end_content_table();
}

void DisplayVisitor::operator()(const Subspecies& subspecies) {
    begin_content_table(subspecies);

    label("Type:");
    ImGui::Text("Subspecies");
    source(subspecies);
    label("Species name:");
    ImGui::Text("%s", subspecies.get_species()->get_name().c_str());
    label("Features:");
    list_features<Feature>(*this, subspecies.get_features());

    end_content_table();
}

void DisplayVisitor::operator()(const Item& item) {
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

void DisplayVisitor::operator()(const Spell& spell) {
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

void DisplayVisitor::operator()(const Feature& feature) {
    begin_content_table(feature);

    label("Type:");
    ImGui::Text("Feature");
    source(feature);
    label("Description:");
    display_formatted_text(feature.get_description());

    end_content_table();
}

void DisplayVisitor::operator()(const Choosable& choosable) {
    begin_content_table(choosable);

    label("Type:");
    ImGui::Text("Choosable Feature - %s", choosable.get_type().c_str());
    source(choosable);
    label("Description:");
    display_formatted_text(choosable.get_description());

    end_content_table();
}

} // namespace dnd
