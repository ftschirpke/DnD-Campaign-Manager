#include <dnd_config.hpp>

#include "command_line_output.hpp"

#include <iostream>
#include <string_view>

void dnd::CommandLineOutput::text(const char* text) { std::cout << text << '\n'; }

void dnd::CommandLineOutput::text(std::string_view text) { std::cout << text << '\n'; }

void dnd::CommandLineOutput::error(const char* error_msg) { std::cerr << error_msg << '\n'; }

void dnd::CommandLineOutput::error(std::string_view error_msg) { std::cerr << error_msg << '\n'; }

void dnd::CommandLineOutput::prompt_input(std::string_view prompt_msg, std::string& out) {
    std::cout << prompt_msg << '\n';
    std::cin >> out;
}
