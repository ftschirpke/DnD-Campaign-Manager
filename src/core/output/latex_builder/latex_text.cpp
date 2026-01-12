#include <dnd_config.hpp>

#include "latex_text.hpp"

#include <cassert>
#include <string>
#include <utility>

namespace dnd {

LatexText::LatexText(const std::string& text) : end_with_newline(true), linebreak(false), text(text) {}

LatexText* LatexText::with_ending_new_line() {
    end_with_newline = true;
    return this;
}

LatexText* LatexText::no_ending_new_line() {
    end_with_newline = false;
    return this;
}

LatexText* LatexText::add_line_break() {
    linebreak = true;
    return this;
}

LatexText* LatexText::add_line_break(const std::string& spacing_argument) {
    linebreak = true;
    linebreak_spacing_argument = spacing_argument;
    return this;
}

LatexText* LatexText::add_modifier(LatexTextModifier modifier) {
    switch (modifier) {
        case LatexTextModifier::ITALIC:
            modifiers.insert("textit");
            break;
        case LatexTextModifier::BOLD:
            modifiers.insert("textbf");
            break;
        case LatexTextModifier::UNDERLINED:
            modifiers.insert("underline");
            break;
        case LatexTextModifier::EMPHASIZED:
            modifiers.insert("emph");
            break;
        default:
            std::unreachable();
    }
    return this;
}

LatexText* LatexText::add_custom_modifier(const std::string& modifier) {
    modifiers.insert(modifier);
    return this;
}

LatexText* LatexText::add_custom_inline_modifier(const std::string& modifier) {
    inline_modifiers.insert(modifier);
    return this;
}

LatexText* LatexText::set_size(const std::string& new_size) {
    this->size = new_size;
    return this;
}

std::string LatexText::str() const {
    std::string text_string;
    for (const std::string& modifier : modifiers) {
        text_string += '\\' + modifier + '{';
    }
    if (size.has_value()) {
        text_string += '\\' + size.value() + ' ';
    }
    for (const std::string& modifier : inline_modifiers) {
        text_string += '\\' + modifier + ' ';
    }
    text_string += text;
    for (const std::string& modifier : modifiers) {
        DND_UNUSED(modifier);
        text_string += '}';
    }
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

std::string LatexText::get_text() const { return text; }

void LatexText::set_text(const std::string& new_text) { text = new_text; }

size_t LatexText::text_size() const { return text.size(); }

} // namespace dnd
