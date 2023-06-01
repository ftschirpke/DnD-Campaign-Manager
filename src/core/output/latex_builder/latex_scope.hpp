#ifndef LATEX_SCOPE_HPP_
#define LATEX_SCOPE_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core/output/latex_builder/latex_command.hpp"
#include "core/output/latex_builder/latex_object.hpp"
#include "core/output/latex_builder/latex_text.hpp"

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
    /**
     * @brief Removes the enclosing braces around this scope
     * @return this scope
     */
    LatexScope* noEnclosingBraces();
    /**
     * @brief Adds a scope to the body of this scope
     * @return the created scope
     */
    LatexScope* addScope();
    /**
     * @brief Adds a text block to the body of this scope
     * @return this scope
     */
    LatexText* addText(const std::string& text);
    /**
     * @brief Add a line break to the body of this scope
     * @return this LaTeX text object
     */
    LatexScope* addLineBreak();
    /**
     * @brief Add a line break to the body of this scope
     * @param modifier the spacing argument for the line break
     * @return this LaTeX text object
     */
    LatexScope* addLineBreak(const std::string& argument);
    /**
     * @brief Adds a command to the body of this scope
     * @param name the name of the command
     * @return the created command
     */
    LatexCommand* addCommand(const std::string& name);
    /**
     * @brief Adds a command with one brace-argument to the body of this scope
     * equivalent to addCommand(name).addBraceArgument(argument)
     * @param name the name of the command
     * @param argument the brace argument for the new command
     * @return the created command
     */
    LatexCommand* addCommand(const std::string& name, const std::string& argument);
    /**
     * @brief Adds a begin-end-block to the body of this scope
     * @param name the name of the begin-end-block
     * @return a pair containing the begin-command and the scope of the block
     */
    LatexBeginEnd addBeginEnd(const std::string& name);
    /**
     * @brief Creates the LaTeX string for this scope
     * @return the LaTeX string
     */
    std::string str() const override;
private:
    bool enclosing_braces;
    std::vector<std::unique_ptr<LatexObject>> objects;
};

inline LatexScope::LatexScope() : enclosing_braces(true) {}

inline LatexCommand* LatexScope::addCommand(const std::string& name, const std::string& argument) {
    return addCommand(name)->addBraceArgument(argument);
}

inline LatexScope* LatexScope::addLineBreak() {
    addText("")->addLineBreak();
    return this;
}

inline LatexScope* LatexScope::addLineBreak(const std::string& argument) {
    addText("")->addLineBreak(argument);
    return this;
}

} // namespace dnd

#endif // LATEX_SCOPE_HPP_
