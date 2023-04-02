#include "dnd_config.hpp"

#include "spell_card_builder.hpp"

#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <iostream>

#include "models/spell.hpp"
#include "output/latex_builder/latex_command.hpp"
#include "output/latex_builder/latex_document.hpp"
#include "output/latex_builder/latex_scope.hpp"
#include "output/latex_builder/latex_text.hpp"

constexpr int card_character_cutoff = 750;

void dnd::SpellCardBuilder::addSpell(const Spell* spell) { spells.push_back(spell); }

void dnd::SpellCardBuilder::writeLatexFile() {
    std::stringstream sstr;
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    sstr << std::put_time(std::localtime(&t), "%F %T\n\n") << ".tex";
    writeLatexFile(sstr.str());
}

void createHeader(dnd::LatexDocument& document) {
    document.document_class.addBracketArgument("parskip");
    document.usePackage("geometry");
    document.usePackage("tcolorbox")->addBracketArgument("most");

    document.header.addCommand("pagenumbering", "gobble");
    document.header.addCommand("geometry", "a4paper,left=3mm,right=3mm,top=3mm,headheight=0mm,headsep=0mm,bottom=3mm");
    document.header.addCommand("makeatletter");
    document.header.addCommand("newcommand");
    document.header.addCommand("notsotiny", "\\@setfontsize\\notsotiny{7.5}{8.5}");
    document.header.addCommand("makeatother");
}

dnd::LatexScope* createCardPage(dnd::LatexDocument& document) {
    std::string colour = "white";
    auto begin_end = document.body.addBeginEnd("tcbitemize");
    begin_end.begin_command->addBracketArgument(
        "size=fbox,raster height=\\textheight,raster columns=3, raster equal skip=5mm,raster rows=3,enhanced,sharp "
        "corners,colback="
        + colour + "!10,colframe=" + colour + "!50!black,text fill"
    );
    return begin_end.scope;
}

void createMinipage(dnd::LatexScope* scope, const std::string& name, const std::string& value) {
    auto minipage = scope->addBeginEnd("minipage");
    minipage.begin_command->addBraceArgument("0.49\\textwidth");
    minipage.scope->addCommand("centering");
    minipage.scope->addCommand("footnotesize");
    minipage.scope->addScope()->addText(name)->addModifier("scshape");
    minipage.scope->addLineBreak();
    minipage.scope->addText(value)->addModifier("scriptsize");
}

dnd::LatexText* createCardHeader(dnd::LatexScope* scope, const dnd::Spell* spell, int counter) {
    scope->addCommand("tcbitem");
    scope->addCommand("vspace", "1mm");
    dnd::LatexScope* center_scope = scope->addBeginEnd("center").scope;
    center_scope->addCommand("MakeUppercase");
    dnd::LatexScope* sub_scope = center_scope->addScope();
    sub_scope->addCommand("textbf");
    dnd::LatexText* title = sub_scope->addScope()->addText(spell->name + " (" + std::to_string(counter) + ')');
    scope->addCommand("vspace", "-3mm");
    createMinipage(scope, "Casting Time", spell->casting_time);
    createMinipage(scope, "Range", spell->range);
    scope->addLineBreak("4pt");
    createMinipage(scope, "Components", spell->components.shortStr());
    createMinipage(scope, "Duration", spell->duration);
    scope->addLineBreak("8pt");
    if (spell->components.material && !spell->components.materials_needed.empty()) {
        scope->addCommand("vspace", "-8mm");
        scope->addBeginEnd("center")
            .scope->addText('(' + spell->components.materials_needed + ')')
            ->addModifier("scriptsize");
        scope->addCommand("vspace", "-2mm");
    }
    scope->addCommand("scriptsize");
    return title;
}

void createCardFooter(dnd::LatexScope* scope, const dnd::Spell* spell) {
    scope->addCommand("vfill");
    scope->addText(spell->type.str())->addModifier("scriptsize")->addModifier("centering");
}

int createSpellCards(dnd::LatexScope* scope, const dnd::Spell* spell) {
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

            dnd::LatexText* text = scope->addText(spell->description.substr(start, end - start));
            characters_written += end - start;
            if (end + 1 < spell->description.size() && spell->description[end + 1] == '\n') {
                text->addLineBreak();
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
    scope->addText(spell->description.substr(start, end - start));
    createCardFooter(scope, spell);

    if (counter == 1) {
        first_title->setText(spell->name);
    }
    return counter;
}

int cardsToCreate(const dnd::Spell* spell) {
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
    // build the latex document
    LatexDocument document("scrartcl");
    createHeader(document);

    std::unordered_map<int, std::deque<dnd::LatexScope*>> not_full_scopes;
    not_full_scopes[9].push_back(createCardPage(document));

    for (const Spell* spell : spells) {
        int cards_to_create = cardsToCreate(spell);
        LatexScope* scope = nullptr;

        int open_slots_before;
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
            std::cout << cards_created << " vs. " << cards_to_create << '\n';
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
