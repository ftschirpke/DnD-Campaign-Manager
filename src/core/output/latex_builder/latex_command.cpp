#include <dnd_config.hpp>

#include "latex_command.hpp"

#include <string>
#include <vector>

dnd::LatexCommand* dnd::LatexCommand::add_brace_argument(const std::string& argument) {
    brace_arguments.push_back(argument);
    return this;
}

dnd::LatexCommand* dnd::LatexCommand::add_bracket_argument(const std::string& argument) {
    bracket_arguments.push_back(argument);
    return this;
}

std::string dnd::LatexCommand::str() const {
    std::string command_string = "\\" + name;
    for (const std::string& bracket_arg : bracket_arguments) {
        command_string += '[' + bracket_arg + ']';
    }
    for (const std::string& brace_arg : brace_arguments) {
        command_string += '{' + brace_arg + '}';
    }
    command_string += '\n';
    return command_string;
}
