#ifndef LATEX_TEXT_HPP_
#define LATEX_TEXT_HPP_

#include <dnd_config.hpp>

#include <optional>
#include <set>
#include <string>

#include <core/output/latex_builder/latex_object.hpp>

namespace dnd {

enum class LatexTextModifier {
    BOLD,
    ITALIC,
    UNDERLINED,
    EMPHASIZED,
};

class LatexText : public LatexObject {
public:
    LatexText(const std::string& text);
    LatexText* with_ending_new_line();
    LatexText* no_ending_new_line();
    LatexText* add_line_break();
    LatexText* add_line_break(const std::string& spacing_argument);
    LatexText* add_modifier(LatexTextModifier modifier);
    LatexText* add_custom_modifier(const std::string& modifier);
    LatexText* add_custom_inline_modifier(const std::string& modifier);
    LatexText* set_size(const std::string& new_size);
    std::string str() const override;
    size_t text_size() const override;
    std::string get_text() const;
    void set_text(const std::string& new_text);
private:
    bool end_with_newline;
    bool linebreak;
    std::string linebreak_spacing_argument;
    std::string text;
    std::set<std::string> modifiers;
    std::set<std::string> inline_modifiers;
    std::optional<std::string> size;
};

} // namespace dnd

#endif // LATEX_TEXT_HPP_
