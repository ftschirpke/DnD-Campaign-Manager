#include <dnd_config.hpp>

#include "command_line_output.hpp"

#include <iostream>
#include <string_view>

namespace dnd {

void CommandLineOutput::newline() const { std::cout << '\n'; }

void CommandLineOutput::text(const char* text) const { std::cout << text << '\n'; }

void CommandLineOutput::text(std::string_view text) const { std::cout << text << '\n'; }

void CommandLineOutput::error(const char* error_msg) const { std::cerr << error_msg << '\n'; }

void CommandLineOutput::error(std::string_view error_msg) const { std::cerr << error_msg << '\n'; }

void CommandLineOutput::prompt_input(std::string_view prompt_msg, std::string& out) const {
    std::cout << prompt_msg << '\n';
    std::getline(std::cin, out);
}

} // namespace dnd
