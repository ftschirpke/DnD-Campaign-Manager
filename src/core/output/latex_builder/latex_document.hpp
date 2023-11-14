#ifndef LATEX_DOCUMENT_HPP_
#define LATEX_DOCUMENT_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/output/latex_builder/latex_command.hpp>
#include <core/output/latex_builder/latex_scope.hpp>

namespace dnd {

class LatexDocument {
public:
    LatexDocument(const std::string& document_class_type);
    LatexCommand* use_package(const std::string& package_name);
    std::string str() const;

    // the header (everything outside the begin{document}-end{document}-block) should contain all the definitions
    LatexScope header;
    // the body (everything inside the begin{document}-end{document}-block) should contain the content of the document
    LatexScope body;
    // the command that defines the class of the document
    LatexCommand document_class;
};

} // namespace dnd

#endif // LATEX_DOCUMENT_HPP_
