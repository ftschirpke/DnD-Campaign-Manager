#include <dnd_config.hpp>

#include "spell_card_builder.hpp"

#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <core/models/spell.hpp>
#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_document.hpp>
#include <core/output/latex_builder/latex_scope.hpp>
#include <core/output/latex_builder/latex_text.hpp>

constexpr int card_character_cutoff = 750;

void dnd::SpellCardBuilder::addSpell(const Spell* spell) { spells.push_back(spell); }

void dnd::SpellCardBuilder::writeLatexFile() {
    std::stringstream sstr;
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    sstr << std::put_time(std::localtime(&t), "%F %T\n\n") << ".tex";
    writeLatexFile(sstr.str());
}

static void createHeader(dnd::LatexDocument& document) {
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

static dnd::LatexScope* createCardPage(dnd::LatexDocument& document) {
    std::string colour = "white";
    auto begin_end = document.body.add_begin_end("tcbitemize");
    begin_end.begin_command->add_bracket_argument(
        "size=fbox,raster height=\\textheight,raster columns=3, raster equal skip=5mm,raster rows=3,enhanced,sharp "
        "corners,colback="
        + colour + "!10,colframe=" + colour + "!50!black,text fill"
    );
    return begin_end.scope;
}

static void createMinipage(dnd::LatexScope* scope, const std::string& name, const std::string& value) {
    auto minipage = scope->add_begin_end("minipage");
    minipage.begin_command->add_brace_argument("0.49\\textwidth");
    minipage.scope->add_command("centering");
    minipage.scope->add_command("footnotesize");
    minipage.scope->add_scope()->add_text(name)->add_modifier("scshape");
    minipage.scope->add_line_break();
    minipage.scope->add_text(value)->add_modifier("scriptsize");
}

static dnd::LatexText* createCardHeader(dnd::LatexScope* scope, const dnd::Spell* spell, int counter) {
    scope->add_command("tcbitem");
    scope->add_command("vspace", "1mm");
    dnd::LatexScope* center_scope = scope->add_begin_end("center").scope;
    center_scope->add_command("MakeUppercase");
    dnd::LatexScope* sub_scope = center_scope->add_scope();
    sub_scope->add_command("textbf");
    dnd::LatexText* title = sub_scope->add_scope()->add_text(spell->name + " (" + std::to_string(counter) + ')');
    scope->add_command("vspace", "-3mm");
    createMinipage(scope, "Casting Time", spell->casting_time);
    createMinipage(scope, "Range", spell->range);
    scope->add_line_break("4pt");
    createMinipage(scope, "Components", spell->components.short_str());
    createMinipage(scope, "Duration", spell->duration);
    scope->add_line_break("8pt");
    if (spell->components.material && !spell->components.materials_needed.empty()) {
        scope->add_command("vspace", "-8mm");
        scope->add_begin_end("center")
            .scope->add_text('(' + spell->components.materials_needed + ')')
            ->add_modifier("scriptsize");
        scope->add_command("vspace", "-2mm");
    }
    scope->add_command("scriptsize");
    return title;
}

static void createCardFooter(dnd::LatexScope* scope, const dnd::Spell* spell) {
    scope->add_command("vfill");
    scope->add_text(spell->type.str())->add_modifier("scriptsize")->add_modifier("centering");
}

static int createSpellCards(dnd::LatexScope* scope, const dnd::Spell* spell) {
    int counter = 1;
    dnd::LatexText* first_title = createCardHeader(scope, spell, counter);
    size_t start = 0;
    size_t end = 0;
    size_t characters_written = 0;
    while (end < spell->description.size()) {
        if (spell->description[end] == '\n') {
            if (characters_written + end - start > card_character_cutoff) {
                // end last card, and start a new card
                createCardFooter(scope, spell);
                createCardHeader(scope, spell, ++counter);
                characters_written = 0;
            }

            dnd::LatexText* text = scope->add_text(spell->description.substr(start, end - start));
            characters_written += end - start;
            if (end + 1 < spell->description.size() && spell->description[end + 1] == '\n') {
                text->add_line_break();
                end++;
            }
            start = ++end;
        }
        end++;
    }
    if (characters_written + end - start > card_character_cutoff) {
        // end last card, and start a new card
        createCardFooter(scope, spell);
        createCardHeader(scope, spell, ++counter);
    }
    scope->add_text(spell->description.substr(start, end - start));
    createCardFooter(scope, spell);

    if (counter == 1) {
        first_title->set_text(spell->name);
    }
    return counter;
}

static int cardsToCreate(const dnd::Spell* spell) {
    int counter = 1;
    size_t start = 0;
    size_t end = 0;
    size_t characters_written = 0;
    while (end < spell->description.size()) {
        if (spell->description[end] == '\n') {
            if (characters_written + end - start > card_character_cutoff) {
                counter++;
                characters_written = 0;
            }

            characters_written += end - start;
            if (end + 1 < spell->description.size() && spell->description[end + 1] == '\n') {
                end++;
            }
            start = ++end;
        }
        end++;
    }
    if (characters_written + end - start > card_character_cutoff) {
        counter++;
    }
    return counter;
}

void dnd::SpellCardBuilder::writeLatexFile(const std::string& filename) {
    LatexDocument document("scrartcl");
    createHeader(document);

    std::unordered_map<int, std::deque<dnd::LatexScope*>> not_full_scopes;
    not_full_scopes[9].push_back(createCardPage(document));

    for (const Spell* spell : spells) {
        int cards_to_create = cardsToCreate(spell);
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
            scope = createCardPage(document);
            open_slots_before = 9;
        }

        int cards_created = createSpellCards(scope, spell);
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
