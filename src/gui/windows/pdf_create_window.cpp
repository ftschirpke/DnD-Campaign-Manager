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
    ItemCardBuilder& item_card_builder;
    SpellCardBuilder& spell_card_builder;
public:
    ParseOpenContentVisitor(ItemCardBuilder& item_card_builder, SpellCardBuilder& spell_card_builder)
        : item_card_builder(item_card_builder), spell_card_builder(spell_card_builder) {}

#define X(C, U, j, a, p, P)                                                                                            \
    void operator()(const C& a) override { DND_UNUSED(a); }
    X_CONTENT_PIECES
#undef X

    void parse(const std::deque<const ContentPiece*>& content_pieces) {
        item_card_builder.clear_items();
        spell_card_builder.clear_spells();
        for (const ContentPiece* content_piece : content_pieces) {
            content_piece->accept_visitor(*this);
        }
    }
};

PdfCreateWindow::PdfCreateWindow(Session& session)
    : session(session), last_content_pieces(std::nullopt), creation_type(PdfCreationType::SPELL_CARDS),
      item_card_builder(), spell_card_builder(), list_items_visitor(session.get_content()),
      list_spells_visitor(session.get_content()) {}

void PdfCreateWindow::render() {
    DND_MEASURE_FUNCTION();
    ImGui::Begin("PDF Creator");

    int* radio_creation_type = reinterpret_cast<int*>(&creation_type);

    ImGui::RadioButton("Spell Cards", radio_creation_type, static_cast<int>(PdfCreationType::SPELL_CARDS));
    ImGui::SameLine();
    ImGui::RadioButton("Item Cards", radio_creation_type, static_cast<int>(PdfCreationType::ITEM_CARDS));

    std::deque<const ContentPiece*>& content_pieces = session.get_open_content_pieces();
    if (!last_content_pieces.has_value() || last_content_pieces.value() != content_pieces) {
        ParseOpenContentVisitor visitor(item_card_builder, spell_card_builder);
        visitor.parse(content_pieces);

        list_items_visitor.clear_list();
        for (const Item& item : item_card_builder.get_items()) {
            item.accept_visitor(list_items_visitor);
        }
        list_spells_visitor.clear_list();
        for (const Spell& spell : spell_card_builder.get_spells()) {
            spell.accept_visitor(list_spells_visitor);
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
