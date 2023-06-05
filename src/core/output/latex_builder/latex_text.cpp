#include <dnd_config.hpp>

#include "latex_text.hpp"

#include <string>
#include <vector>

dnd::LatexText* dnd::LatexText::addModifier(const std::string& modifier) {
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
        if (!linebreak_argument.empty()) {
            text_string += '[' + linebreak_argument + ']';
        }
    }
    if (end_with_newline) {
        text_string += '\n';
    }
    return text_string;
}
