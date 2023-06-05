#include <dnd_config.hpp>

#include "command_line_output.hpp"

#include <iostream>
#include <string_view>

#include <core/models/effect_holder/choosable.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/item.hpp>
#include <core/models/spell.hpp>
#include <core/output/output.hpp>

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

void dnd::CommandLineOutput::display(const dnd::Feature* feature) {
    std::cout << feature->name << '\n' << feature->description << '\n';
}

void dnd::CommandLineOutput::display(const dnd::Choosable* choosable) {
    std::cout << choosable->name << '\n' << choosable->description << '\n';
}

void dnd::CommandLineOutput::prompt_input(std::string_view prompt_msg, std::string& out) {
    std::cout << prompt_msg << '\n';
    std::cin >> out;
}
