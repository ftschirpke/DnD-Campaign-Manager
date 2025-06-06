#include <dnd_config.hpp>

#include "display_visitor.hpp"

#include <set>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <imgui/imgui.h>

#include <constants.hpp>
#include <core/attribute_names.hpp>
#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/character_progression.hpp>
#include <core/basic_mechanics/dice.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/content.hpp>
#include <core/errors/runtime_error.hpp>
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
#include <core/visitors/content/content_visitor.hpp>
#include <gui/gui_fonts.hpp>
#include <gui/visitors/content/display_visitor.hpp>
#include <log.hpp>

namespace dnd {

DisplayVisitor::DisplayVisitor(const Content& content, const GuiFonts& fonts) : content(content), fonts(fonts) {}

static const ImVec2 cell_padding = ImVec2(5, 5);
static constexpr ImGuiTableFlags content_table_flags = ImGuiTableFlags_NoBordersInBodyUntilResize;
static const float first_column_width = 150;

static constexpr ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
                                               | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoHostExtendX;

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
    ImGui::TextWrapped("%s", content_piece.get_source_info().name.c_str());
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

constexpr ImVec4 default_text_color(1.0f, 1.0f, 1.0f, 1.0f);
constexpr ImVec4 link_color(0.309f, 0.712f, 0.847f, 0.784f);

constexpr size_t MAX_ITERATIONS = 1000;

// because we use multiple text calls to support multiple styles,
// the use of the high-level functions ImGui::Text() and ImGui::SameLine() does not allow for the text wrapping we want
// thus, we need to implement it ourselves using ImGui's DrawList by writing one line at a time
static void display_paragraph(const Paragraph& paragraph, const GuiFonts& fonts) {
    float canvas_width = ImGui::GetContentRegionAvail().x;
    ImVec2 begin_cursor = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float x_begin = begin_cursor.x;
    float x_end = x_begin + canvas_width;
    float y_begin = begin_cursor.y;

    float x = x_begin;
    float y = y_begin;

    for (const InlineText& text_obj : paragraph.parts) {
        ImFont* font = nullptr;
        bool push_pop_font = false;
        std::string::const_iterator text_begin;
        std::string::const_iterator text_end;
        bool is_link = false;

        switch (text_obj.index()) {
            case 0: /* SimpleText */ {
                const SimpleText& simple_text = std::get<0>(text_obj);
                font = fonts.get(simple_text.bold, simple_text.italic);
                push_pop_font = ImGui::GetFont() != font;
                text_begin = simple_text.str.cbegin();
                text_end = simple_text.str.cend();
                break;
            }
            case 1: /* Link */ {
                const Link& link = std::get<1>(text_obj);
                font = fonts.get(link.bold, link.italic);
                push_pop_font = ImGui::GetFont() != font;
                text_begin = link.str.cbegin();
                text_end = link.str.cend();
                is_link = true;
                break;
            }
            default: {
                assert(false);
            }
        }

        if (push_pop_font) {
            ImGui::PushFont(font);
        }

        std::string::const_iterator subtext_begin = text_begin;
        std::string::const_iterator current_end = text_begin;
        std::string::const_iterator last_fitting_end;

        size_t outer_iterations = 0;
        bool empty_last_line = false;
        while (subtext_begin != text_end && outer_iterations++ <= MAX_ITERATIONS) {
            ImVec2 subtext_size;
            size_t inner_iterations = 0;
            do {
                if (inner_iterations++ >= MAX_ITERATIONS) {
                    break;
                }
                last_fitting_end = current_end;
                ++current_end;
                if (last_fitting_end == text_end) {
                    break;
                }
                while (current_end != text_end && *current_end != ' ') {
                    ++current_end;
                }
                subtext_size = ImGui::CalcTextSize(&*subtext_begin, &*current_end);
            } while (x + subtext_size.x <= x_end);

            if (inner_iterations++ >= MAX_ITERATIONS) {
                break;
            }

            std::string line_text(subtext_begin, last_fitting_end);
            if (line_text.empty()) {
                if (empty_last_line) {
                    // finding two empty lines in a row is a sign of too little space; aborting...
                    break;
                }
                empty_last_line = true;
            }

            const ImVec4& color = (is_link) ? link_color : default_text_color;
            draw_list->AddText(ImVec2(x, y), ImColor(color), line_text.c_str());

            if (last_fitting_end == text_end) {
                // not reached end of line
                x += subtext_size.x;
                if (subtext_size.y > ImGui::GetTextLineHeight()) {
                    y += subtext_size.y - ImGui::GetTextLineHeight();
                }
            } else {
                // reached end of line
                x = x_begin;
                y += ImGui::GetTextLineHeightWithSpacing();
                if (*last_fitting_end == ' ') {
                    ++last_fitting_end; // declare the found space at line break as "written"
                }
            }

            subtext_begin = last_fitting_end;
            current_end = last_fitting_end;
        }

        if (push_pop_font) {
            ImGui::PopFont();
        }
    }

    if (x != x_begin) {
        y += ImGui::GetTextLineHeightWithSpacing();
    }

    ImVec2 space_used(0, y - y_begin);
    ImGui::Dummy(space_used);
}

static void display_table(const Table& table, const GuiFonts& fonts) {
    const char* id = "unnamed";
    if (table.caption.has_value()) {
        ImGui::PushFont(fonts.bold);
        ImGui::TextWrapped("%s", table.caption.value().c_str());
        ImGui::PopFont();
        id = table.caption.value().c_str();
    }
    if (ImGui::BeginTable(id, static_cast<int>(table.columns), table_flags)) {
        for (size_t col = 0; col < table.columns; ++col) {
            float col_weight = 0;
            if (table.column_widths.has_value()) {
                col_weight = static_cast<float>(table.column_widths->at(col).value_or(0));
            }
            ImGui::TableSetupColumn(table.header.at(col).c_str(), ImGuiTableColumnFlags_WidthStretch, col_weight);
        }
        ImGui::TableHeadersRow();
        for (size_t row = 0; row < table.rows.size(); ++row) {
            ImGui::TableNextRow();
            for (size_t col = 0; col < table.columns; ++col) {
                ImGui::TableSetColumnIndex(static_cast<int>(col));
                if (col >= table.rows[row].size()) {
                    break;
                }
                display_paragraph(table.rows[row][col], fonts);
            }
        }

        ImGui::EndTable();
    }
}

void display_formatted_text(const Text& formatted_text, const GuiFonts& fonts) {
    DND_UNUSED(table_flags);
    ImGui::PushTextWrapPos(0.0f);
    for (const TextObject& text_obj : formatted_text.parts) {
        switch (text_obj.index()) {
            case 0: /* Paragraph */ {
                const Paragraph& paragraph = std::get<0>(text_obj);
                display_paragraph(paragraph, fonts);
                break;
            }
            case 1: /* List */ {
                const List& list = std::get<1>(text_obj);
                if (list.text_above.has_value()) {
                    display_paragraph(list.text_above.value(), fonts);
                }
                for (const ListItem& list_item : list.parts) {
                    ImGui::Bullet();
                    ImGui::BeginGroup();
                    for (const std::variant<Paragraph, Table>& part : list_item.parts) {
                        switch (part.index()) {
                            case 0: /* Paragraph */
                                display_paragraph(std::get<0>(part), fonts);
                                break;
                            case 1: /* Table */
                                display_table(std::get<1>(part), fonts);
                                break;
                            default:
                                assert(false);
                        }
                    }
                    ImGui::EndGroup();
                }
                break;
            }
            case 2: /* Table */ {
                const Table& table = std::get<2>(text_obj);
                display_table(table, fonts);
                break;
            }
            default: {
                assert(false);
            }
        }
    }
    ImGui::PopTextWrapPos();
}

template <typename T>
static void list_features(DisplayVisitor& visitor, const std::vector<T>& features) {
    if (features.empty()) {
        ImGui::Text("None");
        return;
    }
    for (const T& feature : features) {
        ImGui::Separator();
        if (ImGui::TreeNode(feature.get_key().c_str())) {
            ImGui::Separator();
            visitor(feature);
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
}

static void character_abilities_and_skills_table(const dnd::Character& character) {
    DND_UNUSED(character);
    float full_width = ImGui::GetContentRegionAvail().x;
    float per_ability = full_width / 6;
    float big_column = per_ability * 0.618f;
    float small_column = per_ability - big_column;
    big_column = 130.0f;
    small_column = 20.0f;
    if (ImGui::BeginTable("abilities_and_skills_table", 12)) {
        for (const char* ability_cstr : dnd::attributes::ABILITIES) {
            ImGui::TableSetupColumn(ability_cstr, ImGuiTableColumnFlags_WidthFixed, big_column);
            ImGui::TableSetupColumn(
                fmt::format("##{}", ability_cstr).c_str(), ImGuiTableColumnFlags_WidthFixed, small_column
            );
        }
        ImGui::TableNextRow();
        int column = 0;
        for (const char* ability_cstr : dnd::attributes::ABILITIES) {
            ImGui::TableSetColumnIndex(column++);
            ImGui::Text("%s", ability_cstr);
            ImGui::Separator();
            ImGui::TableSetColumnIndex(column++);
            ImGui::Text("  ");
            ImGui::Separator();
        }
        ImGui::TableNextRow();
        column = 0;
        for (Ability ability : abilities_inorder) {
            ImGui::TableSetColumnIndex(column++);
            ImGui::Text("%d", character.get_stats().get_ability_score(ability));
            ImGui::TableSetColumnIndex(column++);
            ImGui::Text("%+d", character.get_stats().get_ability_modifier(ability));
        }
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TableNextRow();
        column = 0;
        for (Ability ability : abilities_inorder) {
            ImGui::TableSetColumnIndex(column++);
            ImGui::Text("Save");
            ImGui::Separator();
            ImGui::TableSetColumnIndex(column++);
            ImGui::Text("%+d", character.get_stats().get_ability_save_modifier(ability));
            ImGui::Separator();
        }
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::TableNextRow();

        std::array<std::vector<std::pair<const char*, int>>, 6> skill_table;
        for (const SkillInfo& skill_info : get_all_skill_infos()) {
            size_t col = 0;
            switch (skill_info.ability) {
                case Ability::STRENGTH:
                    col = 0;
                    break;
                case Ability::DEXTERITY:
                    col = 1;
                    break;
                case Ability::CONSTITUTION:
                    col = 2;
                    break;
                case Ability::INTELLIGENCE:
                    col = 3;
                    break;
                case Ability::WISDOM:
                    col = 4;
                    break;
                case Ability::CHARISMA:
                    col = 5;
                    break;
            }
            int value = character.get_stats().get_skill_modifier(skill_info.skill);
            skill_table[col].emplace_back(skill_info.display_name, value);
        }

        for (size_t row = 0; row < 5; ++row) {
            column = 0;
            for (size_t ability_col_num = 0; ability_col_num < 6; ++ability_col_num) {
                ImGui::TableSetColumnIndex(column++);
                if (row < skill_table[ability_col_num].size()) {
                    std::pair<const char*, int>& name_value_pair = skill_table[ability_col_num][row];
                    ImGui::Text("%s", name_value_pair.first);
                    ImGui::TableSetColumnIndex(column++);
                    ImGui::Text("%+d", name_value_pair.second);
                } else {
                    ImGui::TableSetColumnIndex(column++);
                }
            }
            ImGui::TableNextRow();
        }
        ImGui::TableNextRow();
        ImGui::EndTable();
    }
}

static void character_progression_list(const dnd::Character& character) {
    DND_UNUSED(character);
    const Stats& stats = character.get_stats();
    ImGui::Text("HP: %d/%d", stats.get_current_hp(), stats.get_maximum_hp());
    ImGui::Text("Proficiency Bonus: %+d", character.get_proficiency_bonus());
    ImGui::Text("Armor Class: %d", stats.get_armor_class());
    ImGui::Text("Initiative: %+d", stats.get_initiative());
    ImGui::Text("Speed: %d", stats.get_speed());
    ImGui::Separator();
    int level = character.get_progression().get_level();
    ImGui::Text("Level: %d", level);
    int xp = character.get_progression().get_xp();
    if (level < MAX_CHARACTER_LEVEL) {
        tl::expected<int, RuntimeError> xp_next_level_result = dnd::xp_for_level(level + 1);
        assert(xp_next_level_result.has_value());
        int xp_next_level = xp_next_level_result.value();
        ImGui::Text("XP: %d/%d", xp, xp_next_level);
        ImGui::Text("(%d XP to next level)", xp_next_level - xp);
    } else {
        ImGui::Text("Experience %d", xp);
        ImGui::Text("(Level 20 is the maximum level)");
    }
}

void dnd::DisplayVisitor::operator()(const Character& character) {
    {
        ImGui::BeginChild("abilities_and_skills", ImVec2(ImGui::GetContentRegionAvail().x * 0.8f, 260), false);
        character_abilities_and_skills_table(character);
        ImGui::EndChild();
    }
    ImGui::SameLine();
    {
        ImGui::BeginChild("progression", ImVec2(0, 260), true);
        character_progression_list(character);
        ImGui::EndChild();
    }

    ImGui::Separator();

    begin_content_table(character);

    label("Type:");
    ImGui::Text("Character");
    source(character);

    label("Description:");
    display_formatted_text(character.get_description(), fonts);

    label("Species:");
    const Species& species = character.get_feature_providers().get_species();
    if (ImGui::CollapsingHeader(species.get_key().c_str())) {
        operator()(species);
    }

    OptCRef<Subspecies> subspecies_optional = character.get_feature_providers().get_subspecies();
    if (subspecies_optional.has_value()) {
        label("Subspecies:");
        const Subspecies& subspecies = subspecies_optional.value();
        if (ImGui::CollapsingHeader(subspecies.get_key().c_str())) {
            operator()(subspecies);
        }
    }

    label("Class:");
    const Class& cls = character.get_feature_providers().get_class();
    if (ImGui::CollapsingHeader(cls.get_key().c_str())) {
        operator()(cls);
    }

    OptCRef<Subclass> subclass_optional = character.get_feature_providers().get_subclass();
    if (subclass_optional.has_value()) {
        label("Subclass:");
        const Subclass& subclass = subclass_optional.value();
        if (ImGui::CollapsingHeader(subclass.get_key().c_str())) {
            operator()(subclass);
        }
    }

    label("Features:");
    list_features<Feature>(*this, character.get_features());

    label("Choosables:");
    if (character.get_choosables().empty()) {
        ImGui::Text("None");
    } else {
        for (CRef<Choosable> choosable : character.get_choosables()) {
            ImGui::Separator();
            if (ImGui::TreeNode(choosable.get().get_key().c_str())) {
                ImGui::Separator();
                operator()(choosable.get());
                ImGui::TreePop();
            }
        }
        ImGui::Separator();
    }

    end_content_table();
}

void DisplayVisitor::operator()(const Class& cls) {
    begin_content_table(cls);

    label("Type:");
    ImGui::Text("Class");
    source(cls);
    label("Hit Die:");
    ImGui::Text("%s", cls.get_hit_dice().to_string().c_str());
    label("Feat Levels:");
    const std::set<int>& feat_levels = cls.get_important_levels().get_feat_levels();
    std::string feat_level_str = fmt::format("{}", fmt::join(feat_levels.begin(), feat_levels.end(), ", "));
    ImGui::Text("%s", feat_level_str.c_str());
    label("Subclass Level:");
    ImGui::Text("%d", cls.get_important_levels().get_subclass_level());
    label("Description:");
    display_formatted_text(cls.get_description(), fonts);
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
    ImGui::Text("%s", subclass.get_class().get().get_name().c_str());
    label("Description:");
    display_formatted_text(subclass.get_description(), fonts);
    label("Short name:");
    ImGui::Text("%s", subclass.get_short_name().c_str());
    label("Features:");
    list_features<SubclassFeature>(*this, subclass.get_features());

    end_content_table();
}

void DisplayVisitor::operator()(const Species& species) {
    begin_content_table(species);

    label("Type:");
    ImGui::Text("Species");
    source(species);
    label("Description:");
    display_formatted_text(species.get_description(), fonts);
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
    ImGui::Text("%s", subspecies.get_species().get().get_name().c_str());
    label("Description:");
    display_formatted_text(subspecies.get_description(), fonts);
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
    display_formatted_text(item.get_description(), fonts);
    if (!item.get_cosmetic_description().parts.empty()) {
        wrapped_label("Cosmetic Description:");
        display_formatted_text(item.get_cosmetic_description(), fonts);
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
    label("Concentration:");
    const char* concentration = spell.requires_concentration() ? "yes" : "no";
    ImGui::Text("%s", concentration);
    label("Casting Time:");
    ImGui::Text("%s", spell.get_casting_time().c_str());
    label("Range:");
    ImGui::Text("%s", spell.get_range().c_str());
    label("Components:");
    ImGui::TextWrapped("%s", spell.get_components().str().c_str());
    label("Duration:");
    ImGui::Text("%s", spell.get_duration().c_str());

    label("Description:");
    display_formatted_text(spell.get_description(), fonts);

    end_content_table();
}

void DisplayVisitor::operator()(const Feature& feature) {
    begin_content_table(feature);

    label("Type:");
    ImGui::Text("Feature");
    source(feature);
    label("Description:");
    display_formatted_text(feature.get_description(), fonts);

    end_content_table();
}

void DisplayVisitor::operator()(const ClassFeature& class_feature) {
    begin_content_table(class_feature);

    label("Type:");
    ImGui::Text("Feature");
    source(class_feature);
    DND_UNUSED(content); // TODO: use to lookup content piece for better display
    label("Level:");
    ImGui::Text("%d", class_feature.get_level());
    label("Description:");
    display_formatted_text(class_feature.get_description(), fonts);

    end_content_table();
}

void DisplayVisitor::operator()(const SubclassFeature& subclass_feature) {
    begin_content_table(subclass_feature);

    label("Type:");
    ImGui::Text("Feature");
    source(subclass_feature);
    DND_UNUSED(content); // TODO: use to lookup content piece for better display
    label("Level:");
    ImGui::Text("%d", subclass_feature.get_level());
    label("Description:");
    display_formatted_text(subclass_feature.get_description(), fonts);

    end_content_table();
}

void DisplayVisitor::operator()(const Choosable& choosable) {
    begin_content_table(choosable);

    label("Type:");
    ImGui::Text("Choosable Feature - %s", choosable.get_type().c_str());
    source(choosable);
    label("Description:");
    display_formatted_text(choosable.get_description(), fonts);

    end_content_table();
}

} // namespace dnd
