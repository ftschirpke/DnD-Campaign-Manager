#include <dnd_config.hpp>

#include "latex_document.hpp"

#include <string>

#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_scope.hpp>

namespace dnd {

LatexDocument::LatexDocument(const std::string& document_class_type)
    : header(), body(), document_class("documentclass") {
    header.no_enclosing_braces();
    body.no_enclosing_braces();
    document_class.add_braces_argument(document_class_type);
}

LatexCommand* LatexDocument::use_package(const std::string& package_name) {
    return header.add_command("usepackage", package_name);
}

std::string LatexDocument::str() const {
    std::string document_string;
    document_string.reserve(400); // expect the file size to be large

    document_string += document_class.str();
    document_string += header.str();
    document_string += LatexCommand("begin").add_braces_argument("document")->str();
    document_string += body.str();
    document_string += LatexCommand("end").add_braces_argument("document")->str();

    return document_string;
}

} // namespace dnd
