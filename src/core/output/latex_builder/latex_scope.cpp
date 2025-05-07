#include <dnd_config.hpp>

#include "latex_scope.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_object.hpp>
#include <core/output/latex_builder/latex_rich_text.hpp>
#include <core/output/latex_builder/latex_text.hpp>

namespace dnd {

LatexScope::LatexScope() : enclosing_brace(true) {}

LatexScope* LatexScope::no_enclosing_braces() {
    enclosing_brace = false;
    return this;
}

LatexScope* LatexScope::add_line_break() {
    add_text("")->add_line_break();
    return this;
}

LatexScope* LatexScope::add_line_break(const std::string& spacing_argument) {
    add_text("")->add_line_break(spacing_argument);
    return this;
}

LatexScope* LatexScope::add_scope() {
    auto new_scope = std::make_unique<LatexScope>();
    LatexScope* ptr = new_scope.get();
    objects.push_back(std::move(new_scope));
    return ptr;
}

LatexScope* LatexScope::add_scope(LatexScope&& scope) {
    auto new_scope = std::make_unique<LatexScope>(std::move(scope));
    LatexScope* ptr = new_scope.get();
    objects.push_back(std::move(new_scope));
    return ptr;
}

LatexText* LatexScope::add_rich_text(const std::string& rich_text) {
    std::string deriched_text = derich_text(rich_text);
    return add_text(deriched_text);
}

LatexText* LatexScope::add_text(const std::string& text) {
    auto new_text = std::make_unique<LatexText>(text);
    LatexText* ptr = new_text.get();
    objects.push_back(std::move(new_text));
    return ptr;
}

LatexCommand* LatexScope::add_command(const std::string& name) {
    auto new_command = std::make_unique<LatexCommand>(name);
    LatexCommand* ptr = new_command.get();
    objects.push_back(std::move(new_command));
    return ptr;
}

LatexCommand* LatexScope::add_command(const std::string& name, const std::string& argument) {
    return add_command(name)->add_braces_argument(argument);
}

LatexBeginEnd LatexScope::add_begin_end(const std::string& name) {
    auto begin_command = std::make_unique<LatexCommand>("begin{" + name + '}');
    LatexCommand* begin_ptr = begin_command.get();
    objects.push_back(std::move(begin_command));

    auto begin_end_scope = std::make_unique<LatexScope>();
    begin_end_scope->no_enclosing_braces();
    LatexScope* scope_ptr = begin_end_scope.get();
    objects.push_back(std::move(begin_end_scope));

    auto end_command = std::make_unique<LatexCommand>("end{" + name + '}');
    objects.push_back(std::move(end_command));

    return LatexBeginEnd{begin_ptr, scope_ptr};
}

std::string LatexScope::str() const {
    std::string scope_string;
    scope_string.reserve(100);

    if (enclosing_brace) {
        scope_string += "{\n";
    }
    for (auto it = objects.cbegin(); it != objects.cend(); ++it) {
        scope_string += (*it)->str();
    }
    if (enclosing_brace) {
        scope_string += "}\n";
    }

    return scope_string;
}

size_t LatexScope::text_size() const {
    size_t sum = 0;
    for (auto it = objects.cbegin(); it != objects.cend(); ++it) {
        sum += (*it)->text_size();
    }
    return sum;
}

} // namespace dnd
