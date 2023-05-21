#ifndef LATEX_COMMAND_HPP_
#define LATEX_COMMAND_HPP_

#include "dnd_config.hpp"

#include <string>
#include <vector>

#include "output/latex_builder/latex_object.hpp"

namespace dnd {

class LatexCommand : virtual public LatexObject {
public:
    LatexCommand(const std::string& name);
    virtual ~LatexCommand() = default;
    /**
     * @brief Adds an argument enclosed by braces to the command
     * @param argument the argument
     * @return the command
     */
    LatexCommand* addBraceArgument(const std::string& argument);
    /**
     * @brief Adds and argument enclosed by brackets to the command
     * @param argument the argument
     * @return the command
     */
    LatexCommand* addBracketArgument(const std::string& argument);
    /**
     * @brief Creates the LaTeX string for this command
     * @return the LaTeX string
     */
    std::string str() const override;
private:
    const std::string name;
    std::vector<std::string> brace_arguments;
    std::vector<std::string> bracket_arguments;
};

inline LatexCommand::LatexCommand(const std::string& name) : name(name) {}

} // namespace dnd

#endif // LATEX_COMMAND_HPP_
