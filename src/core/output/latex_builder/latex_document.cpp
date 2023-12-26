#include <dnd_config.hpp>

#include "latex_document.hpp"

#include <string>

#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_scope.hpp>

dnd::LatexDocument::LatexDocument(const std::string& document_class_type)
    : header(), body(), document_class("documentclass") {
    header.no_enclosing_bspecies();
    body.no_enclosing_bspecies();
    document_class.add_bspecies_argument(document_class_type);
}

dnd::LatexCommand* dnd::LatexDocument::use_package(const std::string& package_name) {
    return header.add_command("usepackage", package_name);
}

std::string dnd::LatexDocument::str() const {
    std::string document_string;
    document_string.reserve(400); // expect the file size to be large

    document_string += document_class.str();
    document_string += header.str();
    document_string += LatexCommand("begin").add_bspecies_argument("document")->str();
    document_string += body.str();
    document_string += LatexCommand("end").add_bspecies_argument("document")->str();

    return document_string;
}
