#include "dnd_config.hpp"

#include "command_line_output.hpp"

#include <iostream>
#include <string_view>

#include "models/item.hpp"
#include "models/spell.hpp"
#include "output/output.hpp"

void dnd::CommandLineOutput::text(const char* text) { std::cout << text << '\n'; }

void dnd::CommandLineOutput::text(std::string_view text) { std::cout << text << '\n'; }

void dnd::CommandLineOutput::error(const char* error_msg) { std::cerr << error_msg << '\n'; }

void dnd::CommandLineOutput::error(std::string_view error_msg) { std::cerr << error_msg << '\n'; }

void dnd::CommandLineOutput::display(const dnd::Item* item) {
    std::cout << item->name << '\n' << item->description << '\n';
}

void dnd::CommandLineOutput::display(const dnd::Spell* spell) {
    std::cout << spell->name << '\n' << spell->description << '\n';
}
