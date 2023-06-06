#include <dnd_config.hpp>

#include "latex_document.hpp"

#include <string>

#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_scope.hpp>

std::string dnd::LatexDocument::str() const {
    std::string document_string;
    document_string.reserve(400); // expect the file size to be large

    document_string += document_class.str();
    document_string += header.str();
    document_string += LatexCommand("begin").add_brace_argument("document")->str();
    document_string += body.str();
    document_string += LatexCommand("end").add_brace_argument("document")->str();

    return document_string;
}
