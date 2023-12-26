#include <dnd_config.hpp>

#include "latex_command.hpp"

#include <string>
#include <vector>

dnd::LatexCommand* dnd::LatexCommand::add_bspecies_argument(const std::string& argument) {
    bspecies_arguments.push_back(argument);
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
    for (const std::string& bspecies_arg : bspecies_arguments) {
        command_string += '{' + bspecies_arg + '}';
    }
    command_string += '\n';
    return command_string;
}
