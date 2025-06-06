#include <dnd_config.hpp>

#include "latex_command.hpp"

#include <string>
#include <vector>

namespace dnd {

LatexCommand* LatexCommand::add_braces_argument(const std::string& argument) {
    braces_arguments.push_back(argument);
    return this;
}

LatexCommand* LatexCommand::add_bracket_argument(const std::string& argument) {
    bracket_arguments.push_back(argument);
    return this;
}

std::string LatexCommand::str() const {
    std::string command_string = "\\" + name;
    for (const std::string& bracket_arg : bracket_arguments) {
        command_string += '[' + bracket_arg + ']';
    }
    for (const std::string& bspecies_arg : braces_arguments) {
        command_string += '{' + bspecies_arg + '}';
    }
    command_string += '\n';
    return command_string;
}

size_t LatexCommand::text_size() const { return 0; }

} // namespace dnd
