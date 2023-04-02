#ifndef LATEX_TEXT_HPP_
#define LATEX_TEXT_HPP_

#include "dnd_config.hpp"

#include <string>
#include <vector>

#include "output/latex_builder/latex_object.hpp"

namespace dnd {

/**
 * @brief A class representing a text block in a LaTeX document
 */
class LatexText : public LatexObject {
public:
    LatexText(const std::string& text);
    /**
     * @brief Remove the new line at the end of the text block
     * (useful if you only want differently formatted text in the same paragraph)
     * @return this text block
     */
    LatexText* noEndingNewline();
    /**
     * @brief Add a modifier to the text (e.g. \textit for italics)
     * @param modifier the name of the modifier
     * @return this LaTeX text object
     */
    LatexText* addModifier(const std::string& modifier);
    /**
     * @brief Add a line break to the text (at the end)
     * @return this LaTeX text object
     */
    LatexText* addLineBreak();
    /**
     * @brief Add a line break to the text (at the end)
     * @param modifier the spacing argument for the line break
     * @return this LaTeX text object
     */
    LatexText* addLineBreak(const std::string& argument);
    /**
     * @brief Creates the LaTeX string for this text block
     * @return the LaTeX string
     */
    std::string str() const override;
    std::string getText() const;
    void setText(const std::string& new_text);
private:
    bool end_with_newline;
    bool linebreak;
    std::string linebreak_argument;
    std::string text;
    std::vector<std::string> modifiers;
};

inline LatexText::LatexText(const std::string& text) : end_with_newline(true), linebreak(false), text(text) {}

inline LatexText* LatexText::noEndingNewline() {
    end_with_newline = false;
    return this;
}

inline LatexText* LatexText::addLineBreak() {
    linebreak = true;
    return this;
}

inline LatexText* LatexText::addLineBreak(const std::string& argument) {
    linebreak = true;
    linebreak_argument = argument;
    return this;
}

inline std::string LatexText::getText() const { return text; }

inline void LatexText::setText(const std::string& new_text) { text = new_text; }

} // namespace dnd

#endif // LATEX_TEXT_HPP_
