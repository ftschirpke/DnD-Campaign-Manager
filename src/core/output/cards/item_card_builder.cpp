#include <dnd_config.hpp>

#include "item_card_builder.hpp"

#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <core/content.hpp>
#include <core/models/item/item.hpp>
#include <core/output/latex_builder/latex.hpp>
#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_document.hpp>
#include <core/output/latex_builder/latex_scope.hpp>
#include <core/output/latex_builder/latex_text.hpp>
#include <core/types.hpp>

namespace dnd {

constexpr int card_character_cutoff = 900;

struct ItemScopes {
    std::vector<LatexScope> description_scopes;
    std::vector<LatexScope> cosmetic_scopes;

    static ItemScopes from_item(const Item& item) {
        return ItemScopes{
            .description_scopes = text_to_latex(item.get_description()),
            .cosmetic_scopes = text_to_latex(item.get_cosmetic_description()),
        };
    }
};

ItemCardBuilder::ItemCardBuilder(const Content& content) noexcept : content(content) {}

void ItemCardBuilder::add_item(Id item_id) { items.push_back(item_id); }

std::vector<Id> ItemCardBuilder::get_items() const { return items; }

void ItemCardBuilder::clear_items() { items.clear(); }

std::string ItemCardBuilder::write_latex_file() {
    std::stringstream sstr;
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    sstr << std::put_time(std::localtime(&t), "%F %T") << ".tex";
    write_latex_file(sstr.str());
    return sstr.str();
}

static void create_header(LatexDocument& document) {
    document.document_class.add_bracket_argument("parskip");
    document.use_package("geometry");
    document.use_package("tcolorbox")->add_bracket_argument("most");
    document.use_package("ragged2e");

    document.header.add_command("pagenumbering", "gobble");
    document.header.add_command("geometry", "a4paper,left=3mm,right=3mm,top=3mm,headheight=0mm,headsep=0mm,bottom=3mm");
    document.header.add_command("makeatletter");
    document.header.add_command("newcommand");
    document.header.add_command("notsotiny", "\\@setfontsize\\notsotiny{7.5}{8.5}");
    document.header.add_command("makeatother");
}

static LatexScope* create_card_page(LatexDocument& document) {
    std::string color = "white";
    LatexBeginEnd begin_end = document.body.add_begin_end("tcbitemize");
    begin_end.begin_command->add_bracket_argument(
        "size=fbox,raster height=\\textheight,raster columns=3, raster equal skip=5mm,raster rows=3,enhanced,sharp "
        "corners,colback="
        + color + "!10,colframe=" + color + "!50!black,text fill"
    );
    return begin_end.scope;
}

static LatexText* create_card_header(LatexScope* scope, const Item& item, int counter) {
    scope->add_command("tcbitem");
    scope->add_command("vspace", "1mm");
    LatexScope* center_scope = scope->add_begin_end("center").scope;
    center_scope->add_command("MakeUppercase");
    LatexScope* sub_scope = center_scope->add_scope();
    sub_scope->add_command("textbf");
    LatexText* title = sub_scope->add_scope()->add_text(item.get_name() + " (" + std::to_string(counter) + ')');
    scope->add_command("vspace", "-3mm");
    if (item.requires_attunement()) {
        scope->add_command("vspace", "-5mm");
        scope->add_begin_end("center").scope->add_text("This item requires attunement.")->set_size("scriptsize");
        scope->add_command("vspace", "-3mm");
    }
    scope->add_command("scriptsize");
    return title;
}

static void create_card_footer(LatexScope* scope) { scope->add_command("vspace", "\\fill"); }

static LatexScope* create_textit_scope(LatexScope* scope) {
    LatexScope* center_scope = scope->add_begin_end("Center").scope;
    center_scope->add_command("scriptsize");
    center_scope->add_command("textit");
    return center_scope->add_scope();
}

static int create_item_cards(LatexScope* scope, const Item& item, ItemScopes& scopes) {
    int counter = 1;
    LatexText* first_title = create_card_header(scope, item, counter);
    size_t characters_written = 0;
    for (LatexScope& to_write : scopes.description_scopes) {
        size_t characters = to_write.text_size();
        if (card_character_cutoff - characters_written < characters) {
            create_card_footer(scope);
            create_card_header(scope, item, ++counter);
            characters_written = 0;
        }
        characters_written += characters;
        scope->add_scope(std::move(to_write));
    }

    if (!scopes.cosmetic_scopes.empty()) {
        scope->add_command("vfill");
    }
    scope = create_textit_scope(scope);
    for (LatexScope& to_write : scopes.cosmetic_scopes) {
        size_t characters = to_write.text_size();
        if (card_character_cutoff - characters_written < characters) {
            create_card_footer(scope);
            create_card_header(scope, item, ++counter);
            characters_written = 0;
        }
        characters_written += characters;
        scope->add_scope(std::move(to_write));
    }
    create_card_footer(scope);

    if (counter == 1) {
        first_title->set_text(item.get_name());
    }
    return counter;
}

static int calculate_cards_to_create(const ItemScopes& scopes) {
    int counter = 1;
    size_t characters_written = 0;
    for (const LatexScope& to_write : scopes.description_scopes) {
        size_t characters = to_write.text_size();
        if (card_character_cutoff - characters_written < characters) {
            ++counter;
            characters_written = 0;
        }
        characters_written += characters;
    }

    for (const LatexScope& to_write : scopes.cosmetic_scopes) {
        size_t characters = to_write.text_size();
        if (card_character_cutoff - characters_written < characters) {
            ++counter;
            characters_written = 0;
        }
        characters_written += characters;
    }
    return counter;
}

void ItemCardBuilder::write_latex_file(const std::string& filename) {
    LatexDocument document("scrartcl");
    create_header(document);

    std::unordered_map<int, std::deque<LatexScope*>> not_full_scopes;
    not_full_scopes[9].push_back(create_card_page(document));

    for (Id id : items) {
        const Item& item = content.get_item(id);
        ItemScopes scopes = ItemScopes::from_item(item);
        int cards_to_create = calculate_cards_to_create(scopes);
        LatexScope* scope = nullptr;

        int open_slots_before = -1;
        for (int i = cards_to_create; i <= 9; i++) {
            if (not_full_scopes[i].size() != 0) {
                scope = not_full_scopes[i].front();
                not_full_scopes[i].pop_front();
                open_slots_before = i;
                break;
            }
        }
        if (scope == nullptr) {
            scope = create_card_page(document);
            open_slots_before = 9;
        }

        int cards_created = create_item_cards(scope, item, scopes);
        if (cards_created != cards_to_create) {
            throw std::logic_error("Not yet implemented.");
        }

        int open_slots = open_slots_before - cards_to_create;
        if (open_slots > 0) {
            not_full_scopes[open_slots].push_back(scope);
        }
    }


    std::ofstream output_stream;
    output_stream.open(filename);
    output_stream << document.str();
    output_stream.close();
}

} // namespace dnd
