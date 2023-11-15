#ifndef LATEX_TEXT_HPP_
#define LATEX_TEXT_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/output/latex_builder/latex_object.hpp>

namespace dnd {

class LatexText : public LatexObject {
public:
    LatexText(const std::string& text);
    LatexText* no_ending_new_line();
    LatexText* add_line_break();
    LatexText* add_line_break(const std::string& spacing_argument);
    LatexText* add_modifier(const std::string& modifier);
    std::string str() const override;
    std::string get_text() const;
    void set_text(const std::string& new_text);
private:
    bool end_with_newline;
    bool linebreak;
    std::string linebreak_spacing_argument;
    std::string text;
    std::vector<std::string> modifiers;
};

} // namespace dnd

#endif // LATEX_TEXT_HPP_
