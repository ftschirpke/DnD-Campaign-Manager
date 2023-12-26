#include <dnd_config.hpp>

#include "latex_scope.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_object.hpp>
#include <core/output/latex_builder/latex_text.hpp>

dnd::LatexScope::LatexScope() : enclosing_bspecies(true) {}

dnd::LatexScope* dnd::LatexScope::no_enclosing_bspecies() {
    enclosing_bspecies = false;
    return this;
}

dnd::LatexScope* dnd::LatexScope::add_line_break() {
    add_text("")->add_line_break();
    return this;
}

dnd::LatexScope* dnd::LatexScope::add_line_break(const std::string& spacing_argument) {
    add_text("")->add_line_break(spacing_argument);
    return this;
}

dnd::LatexScope* dnd::LatexScope::add_scope() {
    auto new_scope = std::make_unique<LatexScope>();
    LatexScope* ptr = new_scope.get();
    objects.emplace_back(std::move(new_scope));
    return ptr;
}

dnd::LatexText* dnd::LatexScope::add_text(const std::string& text) {
    auto new_text = std::make_unique<LatexText>(text);
    LatexText* ptr = new_text.get();
    objects.emplace_back(std::move(new_text));
    return ptr;
}

dnd::LatexCommand* dnd::LatexScope::add_command(const std::string& name) {
    auto new_command = std::make_unique<LatexCommand>(name);
    LatexCommand* ptr = new_command.get();
    objects.emplace_back(std::move(new_command));
    return ptr;
}

dnd::LatexCommand* dnd::LatexScope::add_command(const std::string& name, const std::string& argument) {
    return add_command(name)->add_bspecies_argument(argument);
}

dnd::LatexBeginEnd dnd::LatexScope::add_begin_end(const std::string& name) {
    auto begin_command = std::make_unique<LatexCommand>("begin{" + name + '}');
    LatexCommand* begin_ptr = begin_command.get();
    objects.emplace_back(std::move(begin_command));

    auto begin_end_scope = std::make_unique<LatexScope>();
    begin_end_scope->no_enclosing_bspecies();
    LatexScope* scope_ptr = begin_end_scope.get();
    objects.emplace_back(std::move(begin_end_scope));

    auto end_command = std::make_unique<LatexCommand>("end{" + name + '}');
    objects.emplace_back(std::move(end_command));

    return LatexBeginEnd{begin_ptr, scope_ptr};
}

std::string dnd::LatexScope::str() const {
    std::string scope_string;
    scope_string.reserve(100);

    if (enclosing_bspecies) {
        scope_string += "{\n";
    }
    for (auto it = objects.cbegin(); it != objects.cend(); ++it) {
        scope_string += (*it)->str();
    }
    if (enclosing_bspecies) {
        scope_string += "}\n";
    }

    return scope_string;
}
