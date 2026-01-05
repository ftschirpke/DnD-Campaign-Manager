#include <dnd_config.hpp>

#include "latex.hpp"

#include <cassert>

#include <core/output/latex_builder/latex_scope.hpp>
#include <core/text/text.hpp>
#include <log.hpp>

namespace dnd {

static void paragraph_to_latex(const Paragraph& paragraph, LatexScope& scope) {
    LatexText* last_text = nullptr;
    for (const InlineText& text_obj : paragraph.parts) {
        switch (text_obj.index()) {
            case 0: /* SimpleText */ {
                const SimpleText& simple_text = std::get<0>(text_obj);
                LatexText* latex_text = scope.add_text(simple_text.str);
                latex_text->no_ending_new_line();
                if (simple_text.bold) {
                    latex_text->add_modifier(LatexTextModifier::BOLD);
                }
                if (simple_text.italic) {
                    latex_text->add_modifier(LatexTextModifier::ITALIC);
                }
                last_text = latex_text;
                break;
            }
            case 1: /* Link */ {
                const Link& link = std::get<1>(text_obj);
                LatexText* latex_text = scope.add_text(link.text.str);
                latex_text->no_ending_new_line();
                if (link.text.bold) {
                    latex_text->add_modifier(LatexTextModifier::BOLD);
                }
                if (link.text.italic) {
                    latex_text->add_modifier(LatexTextModifier::ITALIC);
                }
                latex_text->add_modifier(LatexTextModifier::EMPHASIZED);
                last_text = latex_text;
                break;
            }
            default: {
                assert(false);
            }
        }
    }
    if (last_text != nullptr) {
        last_text->with_ending_new_line();
        last_text->add_line_break();
    }
}

std::vector<LatexScope> text_to_latex(const Text& text) {
    std::vector<LatexScope> scopes;
    scopes.reserve(text.parts.size());

    for (const TextObject& text_obj : text.parts) {
        LatexScope& scope = scopes.emplace_back();
        switch (text_obj.index()) {
            case 0: /* Paragraph */ {
                const Paragraph& paragraph = std::get<0>(text_obj);
                paragraph_to_latex(paragraph, scope);
                break;
            }
            case 1: /* List */ {
                LOGDEBUG("Unexpected: Found List in Text.");
                break;
            }
            case 2: /* Table */ {
                LOGDEBUG("Unexpected: Found Table in Text.");
                break;
            }
            default: {
                assert(false);
            }
        }
    }

    return scopes;
}

} // namespace dnd
