#include <dnd_config.hpp>

#include "latex_text.hpp"

#include <string>
#include <vector>

dnd::LatexText::LatexText(const std::string& text) : end_with_newline(true), linebreak(false), text(text) {}

dnd::LatexText* dnd::LatexText::no_ending_new_line() {
    end_with_newline = false;
    return this;
}

dnd::LatexText* dnd::LatexText::add_line_break() {
    linebreak = true;
    return this;
}

dnd::LatexText* dnd::LatexText::add_line_break(const std::string& spacing_argument) {
    linebreak = true;
    linebreak_spacing_argument = spacing_argument;
    return this;
}

dnd::LatexText* dnd::LatexText::add_modifier(const std::string& modifier) {
    modifiers.push_back(modifier);
    return this;
}

std::string dnd::LatexText::str() const {
    std::string text_string;
    for (const std::string& modifier : modifiers) {
        text_string += '\\' + modifier + ' ';
    }
    text_string += text;
    if (linebreak) {
        text_string += "\\\\";
        if (!linebreak_spacing_argument.empty()) {
            text_string += '[' + linebreak_spacing_argument + ']';
        }
    }
    if (end_with_newline) {
        text_string += '\n';
    }
    return text_string;
}

std::string dnd::LatexText::get_text() const { return text; }

void dnd::LatexText::set_text(const std::string& new_text) { text = new_text; }
