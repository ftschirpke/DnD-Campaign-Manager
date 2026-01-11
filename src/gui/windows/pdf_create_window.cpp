#include <dnd_config.hpp>

#include "pdf_create_window.hpp"

#include <cassert>
#include <deque>
#include <optional>

#include <imgui/imgui.h>

#include <core/models/content_piece.hpp>
#include <core/session.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

class ParseOpenContentVisitor : public ContentVisitor {
private:
    const Content& content;
    ItemCardBuilder& item_card_builder;
    SpellCardBuilder& spell_card_builder;
public:
    ParseOpenContentVisitor(
        const Content& content, ItemCardBuilder& item_card_builder, SpellCardBuilder& spell_card_builder
    )
        : content(content), item_card_builder(item_card_builder), spell_card_builder(spell_card_builder) {}

#define X(C, U, j, a, p, P)                                                                                            \
    void visit(const C& a) override { DND_UNUSED(a); }
    X_CONTENT_PIECES
#undef X

    void parse(const std::deque<Id>& content_pieces) {
        item_card_builder.clear_items();
        spell_card_builder.clear_spells();
        for (Id id : content_pieces) {
            auto variant = content.get(id);
            visit_variant(variant);
        }
    }
};

PdfCreateWindow::PdfCreateWindow(Session& session)
    : session(session), last_content_pieces(std::nullopt), creation_type(PdfCreationType::SPELL_CARDS),
      item_card_builder(session.get_content()), spell_card_builder(session.get_content()),
      list_items_visitor(session.get_content()), list_spells_visitor(session.get_content()) {}

void PdfCreateWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("PDF Creator");

    int* radio_creation_type = reinterpret_cast<int*>(&creation_type);

    ImGui::RadioButton("Spell Cards", radio_creation_type, static_cast<int>(PdfCreationType::SPELL_CARDS));
    ImGui::SameLine();
    ImGui::RadioButton("Item Cards", radio_creation_type, static_cast<int>(PdfCreationType::ITEM_CARDS));

    std::deque<Id>& content_pieces = session.get_open_content_pieces();
    if (!last_content_pieces.has_value() || last_content_pieces.value() != content_pieces) {
        const Content& content = session.get_content();

        ParseOpenContentVisitor visitor(content, item_card_builder, spell_card_builder);
        visitor.parse(content_pieces);

        list_items_visitor.clear_list();
        for (Id item_id : item_card_builder.get_items()) {
            const Item& item = content.get_item(item_id);
            list_items_visitor.visit(item);
        }
        list_spells_visitor.clear_list();
        for (Id spell_id : spell_card_builder.get_spells()) {
            const Spell& spell = content.get_spell(spell_id);
            list_spells_visitor.visit(spell);
        }
    }


    switch (creation_type) {
        case PdfCreationType::ITEM_CARDS:
            ImGui::SeparatorText("Items to create cards for");
            for (const std::string& str : list_items_visitor.get_list()) {
                ImGui::Text("%s", str.c_str());
            }
            break;
        case PdfCreationType::SPELL_CARDS:
            ImGui::SeparatorText("Spells to create cards for");
            for (const std::string& str : list_spells_visitor.get_list()) {
                ImGui::Text("%s", str.c_str());
            }
            break;
        default:
            assert(false);
    }

    ImGui::Text("When you are happy with your selection press the button to create the cards: ");
    ImGui::SameLine();
    if (ImGui::Button("Create Cards")) {
        switch (creation_type) {
            case PdfCreationType::ITEM_CARDS:
                item_card_builder.write_latex_file(); // TODO: let user choose file name or at least display file name
                break;
            case PdfCreationType::SPELL_CARDS:
                spell_card_builder.write_latex_file(); // TODO: let user choose file name or at least display file name
                break;
            default:
                assert(false);
        }
    }

    ImGui::End();
}

} // namespace dnd
