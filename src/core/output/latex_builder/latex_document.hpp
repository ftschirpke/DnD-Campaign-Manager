#ifndef LATEX_DOCUMENT_HPP_
#define LATEX_DOCUMENT_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_scope.hpp>

namespace dnd {

/**
 * @brief A class representing a LaTeX document consisting of a header and a body
 */
class LatexDocument {
public:
    LatexDocument(const std::string& document_class_type);
    /**
     * @brief Add a usepackage command to the header of the document
     * equivalent to header.addCommand("usepackage", package_name)
     * @param package_name name of the package
     * @return the created usepackage-command
     */
    LatexCommand* usePackage(const std::string& package_name);
    /**
     * @brief Creates the full LaTeX document as a string
     * @return the string
     */
    std::string str() const;
    // the header (everything outside the begin{document}-end{document}-block) should contain all the definitions
    LatexScope header;
    // the body (everything inside the begin{document}-end{document}-block) should contain the content of the document
    LatexScope body;
    // the command that defines the class of the document
    LatexCommand document_class;
};

inline LatexDocument::LatexDocument(const std::string& document_class_type)
    : header(), body(), document_class("documentclass") {
    header.noEnclosingBraces();
    body.noEnclosingBraces();
    document_class.addBraceArgument(document_class_type);
}

inline LatexCommand* LatexDocument::usePackage(const std::string& package_name) {
    return header.addCommand("usepackage", package_name);
}

} // namespace dnd

#endif // LATEX_DOCUMENT_HPP_
