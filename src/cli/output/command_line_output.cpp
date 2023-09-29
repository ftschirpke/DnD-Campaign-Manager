#include <dnd_config.hpp>

#include "command_line_output.hpp"

#include <iostream>
#include <string_view>

#include <core/models/feature/choosable.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/output/output.hpp>

void dnd::CommandLineOutput::text(const char* text) { std::cout << text << '\n'; }

void dnd::CommandLineOutput::text(std::string_view text) { std::cout << text << '\n'; }

void dnd::CommandLineOutput::error(const char* error_msg) { std::cerr << error_msg << '\n'; }

void dnd::CommandLineOutput::error(std::string_view error_msg) { std::cerr << error_msg << '\n'; }

void dnd::CommandLineOutput::display(const dnd::Item* item) {
    std::cout << item->get_name() << '\n' << item->get_description() << '\n';
}

void dnd::CommandLineOutput::display(const dnd::Spell* spell) {
    std::cout << spell->get_name() << '\n' << spell->get_description() << '\n';
}

void dnd::CommandLineOutput::display(const dnd::Feature* feature) {
    std::cout << feature->get_name() << '\n' << feature->get_description() << '\n';
}

void dnd::CommandLineOutput::display(const dnd::Choosable* choosable) {
    std::cout << choosable->get_name() << '\n' << choosable->get_description() << '\n';
}

void dnd::CommandLineOutput::prompt_input(std::string_view prompt_msg, std::string& out) {
    std::cout << prompt_msg << '\n';
    std::cin >> out;
}
