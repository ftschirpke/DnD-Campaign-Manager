#include "dnd_config.hpp"

#include "item_card_builder.hpp"

#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "models/item.hpp"
#include "output/latex_builder/latex_command.hpp"
#include "output/latex_builder/latex_document.hpp"
#include "output/latex_builder/latex_scope.hpp"
#include "output/latex_builder/latex_text.hpp"

constexpr int card_character_cutoff = 900;


void dnd::ItemCardBuilder::addItem(const Item* item) { items.push_back(item); }

void dnd::ItemCardBuilder::writeLatexFile() {
    std::stringstream sstr;
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    sstr << std::put_time(std::localtime(&t), "%F %T\n\n") << ".tex";
    writeLatexFile(sstr.str());
}

static void createHeader(dnd::LatexDocument& document) {
    document.document_class.addBracketArgument("parskip");
    document.usePackage("geometry");
    document.usePackage("tcolorbox")->addBracketArgument("most");
    document.usePackage("ragged2e");

    document.header.addCommand("pagenumbering", "gobble");
    document.header.addCommand("geometry", "a4paper,left=3mm,right=3mm,top=3mm,headheight=0mm,headsep=0mm,bottom=3mm");
    document.header.addCommand("makeatletter");
    document.header.addCommand("newcommand");
    document.header.addCommand("notsotiny", "\\@setfontsize\\notsotiny{7.5}{8.5}");
    document.header.addCommand("makeatother");
}

static dnd::LatexScope* createCardPage(dnd::LatexDocument& document) {
    std::string colour = "white";
    auto begin_end = document.body.addBeginEnd("tcbitemize");
    begin_end.begin_command->addBracketArgument(
        "size=fbox,raster height=\\textheight,raster columns=3, raster equal skip=5mm,raster rows=3,enhanced,sharp "
        "corners,colback="
        + colour + "!10,colframe=" + colour + "!50!black,text fill"
    );
    return begin_end.scope;
}

static dnd::LatexText* createCardHeader(dnd::LatexScope* scope, const dnd::Item* item, int counter) {
    scope->addCommand("tcbitem");
    scope->addCommand("vspace", "1mm");
    dnd::LatexScope* center_scope = scope->addBeginEnd("center").scope;
    center_scope->addCommand("MakeUppercase");
    dnd::LatexScope* sub_scope = center_scope->addScope();
    sub_scope->addCommand("textbf");
    dnd::LatexText* title = sub_scope->addScope()->addText(item->name + " (" + std::to_string(counter) + ')');
    scope->addCommand("vspace", "-3mm");
    if (item->requires_attunement) {
        scope->addCommand("vspace", "-5mm");
        scope->addBeginEnd("center").scope->addText("This item requires attunement.")->addModifier("scriptsize");
        scope->addCommand("vspace", "-3mm");
    }
    scope->addCommand("scriptsize");
    return title;
}

static void createCardFooter(dnd::LatexScope* scope) { scope->addCommand("vspace", "\\fill"); }

static dnd::LatexScope* createTextitScope(dnd::LatexScope* scope) {
    dnd::LatexScope* center_scope = scope->addBeginEnd("Center").scope;
    center_scope->addCommand("scriptsize");
    center_scope->addCommand("textit");
    return center_scope->addScope();
}

static int createItemCards(dnd::LatexScope* scope, const dnd::Item* item) {
    int counter = 1;
    dnd::LatexText* first_title = createCardHeader(scope, item, counter);
    size_t start = 0;
    size_t end = 0;
    size_t characters_written = 0;
    while (end < item->description.size()) {
        if (item->description[end] == '\n') {
            if (characters_written + end - start > card_character_cutoff) {
                // start a new card
                createCardFooter(scope);
                createCardHeader(scope, item, ++counter);
                characters_written = 0;
            }

            dnd::LatexText* text = scope->addText(item->description.substr(start, end - start));
            characters_written += end - start;
            if (end + 1 < item->description.size() && item->description[end + 1] == '\n') {
                text->addLineBreak();
                end++;
            }
            start = ++end;
        }
        end++;
    }
    if (characters_written + end - start > card_character_cutoff) {
        // start a new card
        createCardFooter(scope);
        createCardHeader(scope, item, ++counter);
    }
    scope->addText(item->description.substr(start, end - start));

    int last_description_card = counter;
    bool written_cosmetic_description_yet = false;
    int description_swap_card = -1;
    bool skip_last_footer = false;

    if (!item->cosmetic_description.empty()) {
        start = 0;
        end = 0;
        scope->addCommand("vfill");
        description_swap_card = counter;
        dnd::LatexScope* current_it_scope = createTextitScope(scope);
        while (end < item->cosmetic_description.size()) {
            if (item->cosmetic_description[end] == '\n') {
                if (characters_written + end - start > card_character_cutoff) {
                    // start a new card
                    if (counter != description_swap_card or !written_cosmetic_description_yet) {
                        createCardFooter(scope);
                    }
                    createCardHeader(scope, item, ++counter);
                    current_it_scope = createTextitScope(scope);
                    characters_written = 0;
                }

                if (!written_cosmetic_description_yet && last_description_card == counter) {
                    description_swap_card = counter;
                    written_cosmetic_description_yet = true;
                }
                dnd::LatexText* text = current_it_scope->addText(item->cosmetic_description.substr(start, end - start));

                characters_written += end - start;
                if (end + 1 < item->cosmetic_description.size() && item->cosmetic_description[end + 1] == '\n') {
                    text->addLineBreak();
                    end++;
                }
                start = ++end;
            }
            end++;
        }
        if (characters_written + end - start > card_character_cutoff) {
            // start a new card
            createCardFooter(scope);
            createCardHeader(scope, item, ++counter);
            current_it_scope = createTextitScope(scope);
        }
        if (!written_cosmetic_description_yet) {
            skip_last_footer = true;
        }
        current_it_scope->addText(item->cosmetic_description.substr(start, end - start));
    }
    if (!skip_last_footer) {
        createCardFooter(scope);
    }

    if (counter == 1) {
        first_title->setText(item->name);
    }
    return counter;
}

static int cardsToCreate(const dnd::Item* item) {
    int counter = 1;
    size_t start = 0;
    size_t end = 0;
    size_t characters_written = 0;
    while (end < item->description.size()) {
        if (item->description[end] == '\n') {
            if (characters_written + end - start > card_character_cutoff) {
                counter++;
                characters_written = 0;
            }
            characters_written += end - start;
            if (end + 1 < item->description.size() && item->description[end + 1] == '\n') {
                end++;
            }
            start = ++end;
        }
        end++;
    }
    if (characters_written + end - start > card_character_cutoff) {
        counter++;
    }

    if (!item->cosmetic_description.empty()) {
        start = 0;
        end = 0;
        while (end < item->cosmetic_description.size()) {
            if (item->cosmetic_description[end] == '\n') {
                if (characters_written + end - start > card_character_cutoff) {
                    counter++;
                    characters_written = 0;
                }
                characters_written += end - start;
                if (end + 1 < item->cosmetic_description.size() && item->cosmetic_description[end + 1] == '\n') {
                    end++;
                }
                start = ++end;
            }
            end++;
        }
        if (characters_written + end - start > card_character_cutoff) {
            counter++;
        }
    }
    return counter;
}

void dnd::ItemCardBuilder::writeLatexFile(const std::string& filename) {
    LatexDocument document("scrartcl");
    createHeader(document);

    std::unordered_map<int, std::deque<dnd::LatexScope*>> not_full_scopes;
    not_full_scopes[9].push_back(createCardPage(document));

    for (const Item* item : items) {
        int cards_to_create = cardsToCreate(item);
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

        int cards_created = createItemCards(scope, item);
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
