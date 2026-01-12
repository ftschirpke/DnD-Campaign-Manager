#ifndef PDF_CREATE_WINDOW_HPP_
#define PDF_CREATE_WINDOW_HPP_

#include <dnd_config.hpp>

#include <core/output/cards/item_card_builder.hpp>
#include <core/output/cards/spell_card_builder.hpp>
#include <core/session.hpp>
#include <core/visitors/content/list_content_visitor.hpp>

namespace dnd {

enum class PdfCreationType {
    ITEM_CARDS,
    SPELL_CARDS,
};

class PdfCreateWindow {
public:
    PdfCreateWindow(Session& session);
    void render();
private:
    Session& session;
    std::optional<std::deque<Id>> last_content_pieces;
    PdfCreationType creation_type;
    ItemCardBuilder item_card_builder;
    SpellCardBuilder spell_card_builder;
    ListContentVisitor list_items_visitor;
    ListContentVisitor list_spells_visitor;
};


} // namespace dnd


#endif // PDF_CREATE_WINDOW_HPP_
