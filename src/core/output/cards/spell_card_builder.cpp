#include <dnd_config.hpp>

#include "spell_card_builder.hpp"

#include <chrono>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <core/models/spell/spell.hpp>
#include <core/output/latex_builder/latex.hpp>
#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_document.hpp>
#include <core/output/latex_builder/latex_scope.hpp>
#include <core/output/latex_builder/latex_text.hpp>
#include <core/types.hpp>

namespace dnd {

constexpr int card_character_cutoff = 750;

void SpellCardBuilder::add_spell(const Spell& spell) { spells.push_back(spell); }

void SpellCardBuilder::add_spell(CRef<Spell> spell) { spells.push_back(spell); }

std::vector<CRef<Spell>> SpellCardBuilder::get_spells() const { return spells; }

void SpellCardBuilder::clear_spells() { spells.clear(); }

void SpellCardBuilder::write_latex_file() {
    std::stringstream sstr;
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    sstr << std::put_time(std::localtime(&t), "%F %T") << ".tex";
    write_latex_file(sstr.str());
}

static void create_header(LatexDocument& document) {
    document.document_class.add_bracket_argument("parskip");
    document.use_package("geometry");
    document.use_package("tcolorbox")->add_bracket_argument("most");

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

static void create_minipage(LatexScope* scope, const std::string& name, const std::string& value) {
    LatexBeginEnd minipage = scope->add_begin_end("minipage");
    minipage.begin_command->add_braces_argument("0.49\\textwidth");
    minipage.scope->add_command("centering");
    minipage.scope->add_command("footnotesize");
    minipage.scope->add_scope()->add_text(name)->add_custom_inline_modifier("scshape");
    minipage.scope->add_line_break();
    minipage.scope->add_text(value)->set_size("scriptsize");
}

static LatexText* create_card_header(LatexScope* scope, const Spell& spell, int counter) {
    scope->add_command("tcbitem");
    scope->add_command("vspace", "1mm");
    LatexScope* center_scope = scope->add_begin_end("center").scope;
    center_scope->add_command("MakeUppercase");
    LatexScope* sub_scope = center_scope->add_scope();
    sub_scope->add_command("textbf");
    LatexText* title = sub_scope->add_scope()->add_text(spell.get_name() + " (" + std::to_string(counter) + ')');
    scope->add_command("vspace", "-3mm");
    create_minipage(scope, "Casting Time", spell.get_casting_time());
    create_minipage(scope, "Range", spell.get_range());
    scope->add_line_break("4pt");
    create_minipage(scope, "Components", spell.get_components().short_str());
    create_minipage(scope, "Duration", spell.get_duration());
    scope->add_line_break("8pt");
    if (spell.get_components().has_material() && !spell.get_components().get_material_components().empty()) {
        scope->add_command("vspace", "-8mm");
        scope->add_begin_end("center")
            .scope->add_text('(' + spell.get_components().get_material_components() + ')')
            ->set_size("scriptsize");
        scope->add_command("vspace", "-2mm");
    }
    scope->add_command("scriptsize");
    return title;
}

static void create_card_footer(LatexScope* scope, const Spell& spell) {
    scope->add_command("vfill");
    scope->add_text(spell.get_type().str())->set_size("scriptsize")->add_custom_inline_modifier("centering");
}

static int create_spell_cards(LatexScope* scope, std::vector<LatexScope>& scopes, const Spell& spell) {
    int counter = 1;
    LatexText* first_title = create_card_header(scope, spell, counter);
    size_t characters_written = 0;
    for (LatexScope& to_write : scopes) {
        size_t characters = to_write.text_size();
        if (card_character_cutoff - characters_written < characters) {
            // end last card, and start a new card
            create_card_footer(scope, spell);
            create_card_header(scope, spell, ++counter);
            characters_written = 0;
        }
        characters_written += characters;
        scope->add_scope(std::move(to_write));
    }
    create_card_footer(scope, spell);

    if (counter == 1) {
        first_title->set_text(spell.get_name());
    }
    return counter;
}

static int calculate_cards_to_create(const std::vector<LatexScope>& scopes) {
    int counter = 1;
    size_t characters_written = 0;
    for (const LatexScope& to_write : scopes) {
        size_t characters = to_write.text_size();
        if (card_character_cutoff - characters_written < characters) {
            ++counter;
            characters_written = 0;
        }
        characters_written += characters;
    }
    return counter;
}

void SpellCardBuilder::write_latex_file(const std::string& filename) {
    LatexDocument document("scrartcl");
    create_header(document);

    std::unordered_map<int, std::deque<LatexScope*>> not_full_scopes;
    not_full_scopes[9].push_back(create_card_page(document));

    for (const Spell& spell : spells) {
        std::vector<LatexScope> scopes = text_to_latex(spell.get_description());
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

        int cards_created = create_spell_cards(scope, scopes, spell);
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
