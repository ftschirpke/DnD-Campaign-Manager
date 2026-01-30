#ifndef LATEX_COMMAND_HPP_
#define LATEX_COMMAND_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/output/latex_builder/latex_object.hpp>

namespace dnd {

/*
 * A class designed to only be used for commands modifying text, not containing rendered text itself
 */
class LatexCommand : virtual public LatexObject {
public:
    LatexCommand(const std::string& name);
    virtual ~LatexCommand() = default;
    LatexCommand* add_braces_argument(const std::string& argument);
    LatexCommand* add_bracket_argument(const std::string& argument);
    std::string str() const override final;
    size_t text_size() const override final;
private:
    const std::string name;
    std::vector<std::string> braces_arguments;
    std::vector<std::string> bracket_arguments;
};

inline LatexCommand::LatexCommand(const std::string& name) : name(name) {}

} // namespace dnd

#endif // LATEX_COMMAND_HPP_
