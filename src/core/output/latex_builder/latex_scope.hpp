#ifndef LATEX_SCOPE_HPP_
#define LATEX_SCOPE_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <string>
#include <vector>

#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_object.hpp>
#include <core/output/latex_builder/latex_text.hpp>

namespace dnd {

class LatexScope;

struct LatexBeginEnd {
    LatexCommand* begin_command;
    LatexScope* scope;
};

/**
 * @brief A class representing a scope, a self-contained block of commands and text, within a LaTeX document
 */
class LatexScope : virtual public LatexObject {
public:
    LatexScope();
    LatexScope* no_enclosing_braces();
    LatexScope* add_line_break();
    LatexScope* add_line_break(const std::string& spacing_argument);
    LatexScope* add_scope();
    LatexScope* add_scope(LatexScope&& scope);
    LatexText* add_rich_text(const std::string& rich_text);
    LatexText* add_text(const std::string& text);
    LatexCommand* add_command(const std::string& command);
    LatexCommand* add_command(const std::string& command, const std::string& brace_argument);
    LatexBeginEnd add_begin_end(const std::string& name);
    std::string str() const override;
    size_t text_size() const override;
private:
    bool enclosing_brace;
    std::vector<std::unique_ptr<LatexObject>> objects;
};

} // namespace dnd

#endif // LATEX_SCOPE_HPP_
